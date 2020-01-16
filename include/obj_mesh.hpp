#pragma once

#include <vector>
#include <array>

#include "triangle.hpp"
#include "vec2f.hpp"
#include "vec3f.hpp"

// Simple OBJ mesh file
// assumes triangles

namespace geometry {

struct Indices {

  std::array<unsigned int, 3> id;

  unsigned int operator[](unsigned int idx) const { return id[idx]; }
  unsigned int &operator[](unsigned int idx) { return id[idx]; }

  unsigned int vertexID() const { return id[0]; }
  unsigned int textureCoordID() const { return id[1]; }
  unsigned int normalID() const { return id[2]; }

  unsigned int &vertexID() { return id[0]; }
  unsigned int &textureCoordID() { return id[1]; }
  unsigned int &normalID() { return id[2]; }
};

using IndicesTriangle = Triangle_<Indices>;
using IndicesTriangles = std::vector<IndicesTriangle>;
using Vertices = std::vector<math::Vec3f>;
using Normals = std::vector<math::Vec3f>;
using TextureCoords = std::vector<math::Vec2f>;

struct OBJMesh {
  IndicesTriangles triangles;
  Vertices vertices;
  TextureCoords textureCoords;
  Normals normals;
};

Normals calculateTriangleNormals(IndicesTriangles const &indexTriangles,
                                 Vertices const &vertices);

Normals calculateVertexNormals(IndicesTriangles const &indexTriangles,
                               Vertices const &vertices);

Normals calculateVertexNormals(IndicesTriangles const &indexTriangles,
                               Vertices const &vertices,
                               Normals const &triangleNormals);

} // namespace geometry
