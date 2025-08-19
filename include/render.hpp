#ifndef RENDER_HPP
#define RENDER_HPP

#include <cstdint>
#include "mesh.hpp"

const int WIDTH = 800;
const int HEIGHT = 600;
const float2 SIZE = {WIDTH,HEIGHT};

void render(uint32_t* frame_buffer, float* depth_buffer, const std::vector<Model>& objects,float3 camera_position,  float3 camera_rotation);

#endif
