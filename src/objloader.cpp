#include "objloader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstdarg>

std::vector<Mesh> LoadObjFiles(const std::string &filename...)
{
    std::vector<Mesh> result;
    va_list args;
    va_start(args, filename);
    const char *current_filename_cstr = filename.c_str();
    while (current_filename_cstr != nullptr)
    {
        Mesh mesh;
        std::ifstream file(current_filename_cstr);
        std::string line;

        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v")
            {
                float x, y, z;
                iss >> x >> y >> z;
                mesh.positions.emplace_back(x, y, z);
            }
            else if (prefix == "vt")
            {
                float u, v;
                iss >> u >> v;
                mesh.texcoords.emplace_back(u, v);
            }
            else if (prefix == "vn")
            {
                float x, y, z;
                iss >> x >> y >> z;
                mesh.normals.emplace_back(x, y, z);
            }
            else if (prefix == "f")
            {
                std::vector<Vertex> faceVertices;
                std::string vert;
                while (iss >> vert)
                {
                    std::replace(vert.begin(), vert.end(), '/', ' ');
                    std::istringstream viss(vert);
                    int pi = 0, ti = 0, ni = 0;
                    viss >> pi >> ti >> ni;
                    faceVertices.push_back({pi - 1, ti - 1, ni - 1});
                }
                for (size_t i = 1; i + 1 < faceVertices.size(); ++i)
                {
                    mesh.faces.push_back({faceVertices[0],
                                          faceVertices[i],
                                          faceVertices[i + 1]});
                }
            }
            for (size_t i = 0; i < mesh.faces.size(); ++i)
            {
                mesh.faceColors.push_back(random_color());
            }
        }
        result.push_back(mesh);
        current_filename_cstr = va_arg(args, const char *);
    }
    va_end(args);
    return result;
}
