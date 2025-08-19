#include "transform.hpp"
#include "math.hpp"
#include <render.hpp>
#include <cmath>
#include <tuple>
// Coordinate System (Right-handed):
// X: right, Y: up, Z: forward
std::tuple<mat3, mat3, mat3> GetRotationMatrices(float3 rotation)
{
    mat3 pitch = {
        {1, 0, 0},
        {0, static_cast<float>(cos(rotation.z)), static_cast<float>(-sin(rotation.z))},
        {0, static_cast<float>(sin(rotation.z)), static_cast<float>(cos(rotation.z))},
    };
    mat3 yaw = {
        {static_cast<float>(cos(rotation.y)), 0, static_cast<float>(sin(rotation.y))},
        {0, 1, 0},
        {static_cast<float>(-sin(rotation.y)), 0, static_cast<float>(cos(rotation.y))}};
    mat3 roll = {
        {static_cast<float>(cos(rotation.x)), static_cast<float>(-sin(rotation.x)), 0},
        {static_cast<float>(sin(rotation.x)), static_cast<float>(cos(rotation.x)), 0},
        {0, 0, 1}};
    return {pitch, yaw, roll};
}
float3 ToWorldPoint(float3 p, float3 position, float3 rotation)
{
    mat3 pitch_m, yaw_m, roll_m;
    std::tie(pitch_m, yaw_m, roll_m) = GetRotationMatrices(rotation);
    mat3 r = yaw_m * pitch_m * roll_m;
    return r * p + position;
}
float3 ToLocalPoint(float3 vertex_world, float3 position, float3 rotation)
{
    float3 translated_point = vertex_world - position;
    mat3 pitch_m, yaw_m, roll_m;
    std::tie(pitch_m, yaw_m, roll_m) = GetRotationMatrices(rotation);
    mat3 camera_world_rotation = yaw_m * pitch_m * roll_m;
    mat3 inv_camera_rotation = camera_world_rotation.transpose();
    return inv_camera_rotation * translated_point;
}
float2 ToScreenPoint(float3 vertex_local)
{
    float fov_deg = 60;
    float fov_rad = fov_deg * M_PI / 180.0f;
    float screenHeight_world = tan(fov_rad / 2) * 2;
    if (vertex_local.z <= 0.1f)
        vertex_local.z = 0.1f;
    float pixelsPerWorldUnit = SIZE.y / screenHeight_world / vertex_local.z;

    float2 pixelOffset = float2{vertex_local.x, vertex_local.y} * pixelsPerWorldUnit;
    return (SIZE / 2.0f) + pixelOffset;
}
