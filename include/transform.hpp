#pragma once
#include "math.hpp" 
float2 ToScreenPoint(float3 vertex_local);
std::tuple<mat3, mat3, mat3> GetRotationMatrices(float3 rotation);
float3 ToLocalPoint(float3 vertex_world, float3 position, float3 rotation);
float3 ToWorldPoint(float3 p, float3 position, float3 rotation);