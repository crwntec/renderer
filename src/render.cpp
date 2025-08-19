#include "render.hpp"
#include "math.hpp"
#include <algorithm>
#include <cmath>
#include <transform.hpp>
#include <limits>

const float3 BG = {0, 0, 0};
inline uint32_t pack_color(float3 color, uint8_t a = 255)
{
    auto clamp = [](float x) -> uint8_t
    {
        return static_cast<uint8_t>(x < 0 ? 0 : x > 1 ? 255
                                                      : x * 255);
    };
    return (a << 24) | (clamp(color.r) << 16) | (clamp(color.g) << 8) | clamp(color.b);
}

void clear_framebuffer(uint32_t *buffer)
{
    for (int i = 0; i < WIDTH * HEIGHT; ++i)
        buffer[i] = pack_color(BG);
}
void clear_depthbuffer(float *buffer)
{
    for (int i = 0; i < WIDTH * HEIGHT; ++i)
        buffer[i] = std::numeric_limits<float>::infinity();
}

void draw_pixel(uint32_t *buffer, float2 pos, float3 color)
{
    if (pos.x < 0 || pos.x >= WIDTH || pos.y < 0 || pos.y >= HEIGHT)
        return;
    buffer[static_cast<int>(pos.y) * WIDTH + static_cast<int>(pos.x)] = pack_color(color);
}

void render(uint32_t *frame_buffer, float *depth_buffer, const std::vector<Model> &models, float3 camera_position, float3 camera_rotation)
{
    clear_framebuffer(frame_buffer);
    clear_depthbuffer(depth_buffer);
    std::vector<float2> facePoints;
    for (auto &model : models)
    {
        const auto &mesh = *model.mesh_data;
        for (size_t faceIndex = 0; faceIndex < mesh.faces.size(); ++faceIndex)
        {
            const auto &face = mesh.faces[faceIndex];
            float3 p0_world = ToWorldPoint(mesh.positions[face[0].posIndex], model.position, model.rotation);
            float3 p1_world = ToWorldPoint(mesh.positions[face[1].posIndex], model.position, model.rotation);
            float3 p2_world = ToWorldPoint(mesh.positions[face[2].posIndex], model.position, model.rotation);
            
            float3 p0_local = ToLocalPoint(p0_world, camera_position, camera_rotation);
            float3 p1_local = ToLocalPoint(p1_world, camera_position, camera_rotation);
            float3 p2_local = ToLocalPoint(p2_world, camera_position, camera_rotation);
            if (p0_local.z <= 0 || p1_local.z <= 0 || p2_local.z <= 0) continue;

            float2 a = ToScreenPoint(p0_local);
            float2 b = ToScreenPoint(p1_local);
            float2 c = ToScreenPoint(p2_local);

            float minX = std::floor(std::min({a.x, b.x, c.x}));
            float minY = std::floor(std::min({a.y, b.y, c.y}));
            float maxX = std::ceil(std::max({a.x, b.x, c.x}));
            float maxY = std::ceil(std::max({a.y, b.y, c.y}));

            int startX = std::clamp(static_cast<int>(minX), 0, WIDTH - 1);
            int startY = std::clamp(static_cast<int>(minY), 0, HEIGHT - 1);
            int endX = std::clamp(static_cast<int>(maxX), 0, WIDTH - 1);
            int endY = std::clamp(static_cast<int>(maxY), 0, HEIGHT - 1);

            float3 triColor = mesh.faceColors[faceIndex];

            for (int y = startY; y <= endY; ++y)
            {
                for (int x = startX; x <= endX; ++x)
                {
                    float3 weights;
                    float2 p = {static_cast<float>(x),
                                static_cast<float>(y)};
                    if (MATH::PointInTriangle(a, b, c, p, &weights))
                    {
                        float depths[3] = {p0_world.z, p1_world.z, p2_world.z};
                        float interpolated_depth = weights.x * depths[0] + weights.y * depths[1] + weights.z * depths[2];
                        int buffer_index = static_cast<int>(p.y) * WIDTH + static_cast<int>(p.x);
                        if (interpolated_depth < depth_buffer[buffer_index])
                        {
                            depth_buffer[buffer_index] = interpolated_depth;
                            draw_pixel(frame_buffer, p, triColor);
                        }
                    }
                }
            }
        }
    }
}