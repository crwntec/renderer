#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include "math.hpp"
#include <array>

struct Face
{
    int v1, v2, v3;
};

struct Vertex
{
    int posIndex;
    int texIndex;
    int normIndex;
};

struct Mesh
{
    std::vector<float3> positions;
    std::vector<float2> texcoords;
    std::vector<float3> normals;
    std::vector<std::array<Vertex, 3>> faces;
    std::vector<float3> faceColors;
};

struct Model
{
    Mesh *mesh_data;
    float3 position;
    float3 rotation;
    float3 scale;
    Model(Mesh *mesh, float3 pos = {0, 0, 0}, float3 rot = {0, 0, 0}, float3 s = {1, 1, 1})
        : mesh_data(mesh), position(pos), rotation(rot), scale(s) {}
};

#endif
