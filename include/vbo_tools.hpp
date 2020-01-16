#pragma once

#include <vector>

#include "obj_mesh_file_io.hpp"
#include "vec3f.hpp"
#include "vec2f.hpp"
#include "buffer_object.hpp"
#include "vertex_array_object.hpp"

namespace opengl {

using Indices = std::vector<unsigned int>;

struct VBOData_Vertices {
  Indices indices;
  geometry::Vertices vertices;
};

struct VBOData_VerticesNormals {
  Indices indices;
  geometry::Vertices vertices;
  geometry::Normals normals;
};

struct VBOData_VerticesTexutreCoordsNormals {
  Indices indices;
  geometry::Vertices vertices;
  geometry::TextureCoords textureCoords;
  geometry::Normals normals;
};

// converts OBJ-like data
// (e.g. f 12/3/90 11/4/91 10/20/30 -> 0 [v,n,uv] 1 [v,n,uv] 2 [v,n,uv]
// so as to be condusive for vertex element buffers
VBOData_Vertices makeConsistentVertexIndices(geometry::OBJMesh const &mesh);

VBOData_VerticesNormals
makeConsistentVertexNormalIndices(geometry::OBJMesh const &mesh);

VBOData_VerticesNormals
makeConsistentVertexNormalIndices(geometry::OBJMesh const &mesh,
                                  geometry::Normals vertexNormals);

VBOData_VerticesTexutreCoordsNormals
makeConsistentVertexTextureCoordNormalIndices(geometry::OBJMesh const &mesh);

VBOData_VerticesTexutreCoordsNormals
makeConsistentVertexTextureCoordNormalIndices(
    geometry::OBJMesh const &mesh, geometry::Normals const &vertexNormals);

unsigned int setup_vao_and_buffers(opengl::VertexArrayObject &vao,
                                   opengl::BufferObject &indexBuffer,
                                   opengl::BufferObject &vertexBuffer,
                                   opengl::VBOData_VerticesNormals const &data);

unsigned int
setup_vao_and_buffers(opengl::VertexArrayObject &vao,
                      opengl::BufferObject &indexBuffer,
                      opengl::BufferObject &vertexBuffer,
                      opengl::VBOData_VerticesTexutreCoordsNormals const &data);

} // namespace vbo
