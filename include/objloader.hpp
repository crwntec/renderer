#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include <vector>
#include <string>
#include "mesh.hpp"

std::vector<Mesh> LoadObjFiles(const std::string& filename...);

#endif
