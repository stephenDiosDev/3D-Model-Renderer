#pragma once

#include <string>
#include <array>
#include <vector>

#include "obj_mesh.hpp"
#include "triangle.hpp"
#include "vec3f.hpp"
#include "vec2f.hpp"

namespace geometry {

bool loadOBJMeshFromFile(std::string const &filePath, geometry::OBJMesh &mesh);

} // namespace geometry
