#include "vbo_tools.hpp"

#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <map>

#include "glad/glad.h"

// Wont work for meshes that are in excess of #v * #uv * #n > max(size_t)
// but that is around the mark of 10,000,000 x 10,000,000 x 1,000,000,
// so we are probably fine

namespace opengl {

VBOData_Vertices makeConsistentVertexIndices(geometry::OBJMesh const &mesh) {

  // simply copy the indices..
  std::vector<unsigned int> indices;
  indices.resize(mesh.triangles.size() * 3);

  // strip out just vertex IDs
  for (auto const &t : mesh.triangles) {
    indices.emplace_back(t.a().vertexID());
    indices.emplace_back(t.b().vertexID());
    indices.emplace_back(t.c().vertexID());
  }

  return {indices, mesh.vertices};
}

VBOData_VerticesNormals
makeConsistentVertexNormalIndices(geometry::OBJMesh const &mesh,
                                  geometry::Normals vertexNormals) {

  // simply copy the indices..
  std::vector<unsigned int> indices;
  indices.resize(mesh.triangles.size() * 3);

  // strip out just vertex IDs
  for (auto const &t : mesh.triangles) {
    indices.emplace_back(t.a().vertexID());
    indices.emplace_back(t.b().vertexID());
    indices.emplace_back(t.c().vertexID());
  }

  return {indices, mesh.vertices, vertexNormals};
}

VBOData_VerticesNormals
makeConsistentVertexNormalIndices(geometry::OBJMesh const &mesh) {

  std::unordered_map<size_t, unsigned int> mappedIndices;
  mappedIndices.reserve(mesh.vertices.size());

  std::vector<unsigned int> indicesOut;
  indicesOut.reserve(mesh.triangles.size() *
                     3); // avoid early resize growth penalty

  std::vector<math::Vec3f> verticesOut;
  verticesOut.reserve(mesh.vertices.size()); // at least this many vertices
  std::vector<math::Vec3f> normalsOut;
  normalsOut.reserve(verticesOut.size()); // at least this many normals

  auto verticesIDRange = mesh.vertices.size();
  auto normalsIDRange = mesh.normals.size();
  auto keyGen = [verticesIDRange, normalsIDRange](unsigned int vertexID,
                                                  unsigned int normalID) {
    return vertexID + verticesIDRange * normalID;
  };

  for (auto const &t : mesh.triangles) {

    for (int idx = 0; idx < 3; ++idx) {
      auto index = t[idx];

      auto key = keyGen(index.vertexID(), index.textureCoordID());

      auto iter = mappedIndices.find(key);
      if (iter != mappedIndices.end()) {
        indicesOut.push_back(iter->second); // reuse id
      } else {

        unsigned int id = verticesOut.size();

        indicesOut.push_back(id); // new id
        verticesOut.push_back(mesh.vertices[index.vertexID()]);
        normalsOut.push_back(mesh.normals[index.normalID()]);

        mappedIndices[key] = id; // save for next
      }
    }
  }

  // optional
  indicesOut.shrink_to_fit();
  verticesOut.shrink_to_fit();
  normalsOut.shrink_to_fit();

  return {indicesOut, verticesOut, normalsOut};
}

VBOData_VerticesTexutreCoordsNormals
makeConsistentVertexTextureCoordNormalIndices(
    geometry::OBJMesh const &mesh, geometry::Normals const &vertexNormals) {

  std::unordered_map<size_t, unsigned int> mappedIndices;
  mappedIndices.reserve(mesh.vertices.size());

  std::vector<unsigned int> indicesOut;
  indicesOut.reserve(mesh.triangles.size() *
                     3); // avoid early resize growth penalty

  std::vector<math::Vec3f> verticesOut;
  verticesOut.reserve(mesh.vertices.size()); // at least this many vertices
  std::vector<math::Vec2f> textureCoordsOut;
  textureCoordsOut.reserve(
      mesh.vertices.size()); // at least this many textureCoords
  std::vector<math::Vec3f> normalsOut;
  normalsOut.reserve(vertexNormals.size()); // at least this many normals

  auto verticesIDRange = mesh.vertices.size();
  auto textureCoordsIDRange = mesh.textureCoords.size();
  // auto normalsIDRange = normals.size(); // not needed

  auto keyGen =
      [verticesIDRange, textureCoordsIDRange](
          unsigned int vertexID, unsigned int textureCoordID) -> size_t {
        return vertexID + verticesIDRange * textureCoordID;
      };

  for (auto const &t : mesh.triangles) {

    for (int idx = 0; idx < 3; ++idx) {
      auto index = t[idx];

      auto key = keyGen(index.vertexID(),
                        index.textureCoordID()); // index.normalID());

      auto iter = mappedIndices.find(key);
      if (iter != mappedIndices.end()) {
        auto vID = iter->second; // reuse existing id
        indicesOut.push_back(vID);
      } else {

        unsigned int id = verticesOut.size();

        indicesOut.push_back(id); // new id
        verticesOut.push_back(mesh.vertices[index.vertexID()]);
        textureCoordsOut.push_back(mesh.textureCoords[index.textureCoordID()]);
        normalsOut.push_back(vertexNormals[index.vertexID()]);

        mappedIndices[key] = id;
      }
    }
  }

  // optional
  indicesOut.shrink_to_fit();
  verticesOut.shrink_to_fit();
  textureCoordsOut.shrink_to_fit();
  normalsOut.shrink_to_fit();

  return {indicesOut, verticesOut, textureCoordsOut, normalsOut};
}

VBOData_VerticesTexutreCoordsNormals
makeConsistentVertexTextureCoordNormalIndices(geometry::OBJMesh const &mesh) {

  std::unordered_map<size_t, unsigned int> mappedIndices;
  // std::map<size_t, unsigned int> mappedIndices;
  mappedIndices.reserve(mesh.vertices.size());

  std::vector<unsigned int> indicesOut;
  indicesOut.reserve(mesh.triangles.size() * 3);

  std::vector<math::Vec3f> verticesOut;
  verticesOut.reserve(mesh.vertices.size()); // at least this many vertices
  std::vector<math::Vec2f> textureCoordsOut;
  textureCoordsOut.reserve(mesh.vertices.size()); // at least this many
  std::vector<math::Vec3f> normalsOut;
  normalsOut.reserve(mesh.vertices.size()); // at least this many normals

  auto verticesIDRange = mesh.vertices.size();
  auto textureCoordsIDRange = mesh.textureCoords.size();
  // auto normalsIDRange = normals.size(); // not needed

  auto keyGen = [verticesIDRange, textureCoordsIDRange](
                    unsigned int vertexID, unsigned int textureCoordID,
                    unsigned int normalID) -> size_t {
    return vertexID + verticesIDRange * textureCoordID +
           (verticesIDRange * textureCoordsIDRange) * normalID;
  };

  for (auto const &t : mesh.triangles) {

    for (int idx = 0; idx < 3; ++idx) {
      auto index = t[idx];

      auto key =
          keyGen(index.vertexID(), index.textureCoordID(), index.normalID());

      auto iter = mappedIndices.find(key);
      if (iter != mappedIndices.end()) {
        auto vID = iter->second; // reuse existing id
        indicesOut.push_back(vID);
      } else {

        unsigned int id = verticesOut.size();

        indicesOut.push_back(id); // new id
        verticesOut.push_back(mesh.vertices[index.vertexID()]);
        textureCoordsOut.push_back(mesh.textureCoords[index.textureCoordID()]);
        normalsOut.push_back(mesh.normals[index.normalID()]);

        mappedIndices[key] = id;
      }
    }
  }

  // optional
  indicesOut.shrink_to_fit();
  verticesOut.shrink_to_fit();
  textureCoordsOut.shrink_to_fit();
  normalsOut.shrink_to_fit();

  return {indicesOut, verticesOut, textureCoordsOut, normalsOut};
}

unsigned int setup_vao_and_buffers(opengl::VertexArrayObject &vao,
                                   opengl::BufferObject &indexBuffer,
                                   opengl::BufferObject &vertexBuffer,
                                   opengl::VBOData_Vertices const &data) {
  using namespace opengl;

  vao.bind();

  auto indexSize = sizeof(unsigned int) * (data.indices.size());
  indexBuffer.bind(BufferObject::ELEMENT_ARRAY);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, // type
               indexSize,               // size
               data.indices.data(),     // data
               GL_STATIC_DRAW);

  // set up position input into vertex shader
  auto vertexSize = sizeof(math::Vec3f) * data.vertices.size();
  auto totalSize = vertexSize;

  auto vertexOffset = size_t(0);

  vertexBuffer.bind(BufferObject::ARRAY);

  // positions
  glEnableVertexAttribArray(0); // match layout # in vertex shader
  glVertexAttribPointer(        //
      0,                        // attribute layout # (in shader)
      3,                        // number of coordinates per vertex
      GL_FLOAT,                 // type
      GL_FALSE,                 // normalized?
      sizeof(math::Vec3f),      // stride
      (void *)(vertexOffset)    // array buffer offset
      );

  // request storage, but provide no data
  // [ vertices | normals | uvs ]
  glBufferData(GL_ARRAY_BUFFER, totalSize, NULL, GL_STATIC_DRAW);

  // load positions
  // [ v | v | v | ]
  glBufferSubData(GL_ARRAY_BUFFER,       // type
                  0,                     // offset
                  vertexSize,            // size
                  data.vertices.data()); // data pointer

  vao.unbind();
  indexBuffer.unbind();
  vertexBuffer.unbind();

  return data.indices.size();
}

unsigned int setup_vao_and_buffers(opengl::VertexArrayObject &vao,
                                   opengl::BufferObject &indexBuffer,
                                   opengl::BufferObject &vertexBuffer,
                                   VBOData_VerticesNormals const &data) {
  using namespace opengl;

  vao.bind();

  // bind these indices
  auto indexSize = sizeof(unsigned int) * (data.indices.size());
  indexBuffer.bind(BufferObject::ELEMENT_ARRAY);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, // type
               indexSize,               // size
               data.indices.data(),     // data
               GL_STATIC_DRAW);

  // set up position input into vertex shader
  auto vertexSize = sizeof(math::Vec3f) * data.vertices.size();
  auto normalsSize = sizeof(math::Vec3f) * data.normals.size();
  auto totalSize = vertexSize + normalsSize;

  auto verticesOffset = size_t(0);
  auto normalsOffset = vertexSize;

  vertexBuffer.bind(BufferObject::ARRAY);

  // vao setup
  // positions
  glEnableVertexAttribArray(0); // match layout # in vertex shader
  glVertexAttribPointer(        //
      0,                        // attribute layout # (in shader)
      3,                        // number of coordinates per vertex
      GL_FLOAT,                 // type
      GL_FALSE,                 // normalized?
      sizeof(math::Vec3f),      // stride
      (void *)(verticesOffset)  // array buffer offset
      );

  // normals
  glEnableVertexAttribArray(1); // match layout # in vertex shader
  glVertexAttribPointer(        //
      1,                        // attribute layout # (in shader)
      3,                        // number of coordinates per vertex
      GL_FLOAT,                 // type
      GL_FALSE,                 // normalized?
      sizeof(math::Vec3f),      // stride
      (void *)(normalsOffset)   // array buffer offset
      );

  // request storage, but provide no data
  // [ vertices | normals | uvs ]
  glBufferData(GL_ARRAY_BUFFER, totalSize, NULL, GL_STATIC_DRAW);

  // load positions
  // [ v | v | v | ... | *uninitialized* ]
  glBufferSubData(GL_ARRAY_BUFFER,       // type
                  verticesOffset,        // offset
                  vertexSize,            // size
                  data.vertices.data()); // data pointer

  // load normals
  // [ v | v | v | ... | n | n | n | ]
  glBufferSubData(GL_ARRAY_BUFFER,      // type
                  normalsOffset,        // offset
                  normalsSize,          // size
                  data.normals.data()); // data pointer

  vao.unbind();
  indexBuffer.unbind();
  vertexBuffer.unbind();

  return data.indices.size();
}

unsigned int
setup_vao_and_buffers(opengl::VertexArrayObject &vao,
                      opengl::BufferObject &indexBuffer,
                      opengl::BufferObject &vertexBuffer,
                      VBOData_VerticesTexutreCoordsNormals const &data) {
  using namespace opengl;

  vao.bind();

  // bind these indices
  auto indexSize = sizeof(unsigned int) * (data.indices.size());
  indexBuffer.bind(BufferObject::ELEMENT_ARRAY);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, // type
               indexSize,               // size
               data.indices.data(),     // data
               GL_STATIC_DRAW);

  // set up position input into vertex shader
  auto vertexSize = sizeof(math::Vec3f) * data.vertices.size();
  auto textureCoordSize = sizeof(math::Vec2f) * data.textureCoords.size();
  auto normalsSize = sizeof(math::Vec3f) * data.normals.size();
  auto totalSize = vertexSize + normalsSize + textureCoordSize;

  auto verticesOffset = size_t(0);
  auto normalsOffset = vertexSize;
  auto textureCoordsOffset = vertexSize + normalsSize;

  vertexBuffer.bind(BufferObject::ARRAY);

  // vao setup
  // positions
  glEnableVertexAttribArray(0); // match layout # in vertex shader
  glVertexAttribPointer(        //
      0,                        // attribute layout # (in shader)
      3,                        // number of coordinates per vertex
      GL_FLOAT,                 // type
      GL_FALSE,                 // normalized?
      sizeof(math::Vec3f),      // stride
      (void *)(verticesOffset)  // array buffer offset
      );

  // normals
  glEnableVertexAttribArray(1); // match layout # in vertex shader
  glVertexAttribPointer(        //
      1,                        // attribute layout # (in shader)
      3,                        // number of coordinates per vertex
      GL_FLOAT,                 // type
      GL_FALSE,                 // normalized?
      sizeof(math::Vec3f),      // stride
      (void *)(normalsOffset)   // array buffer offset
      );

  // texture coords
  glEnableVertexAttribArray(2);     // match layout # in vertex shader
  glVertexAttribPointer(            //
      2,                            // attribute layout # (in shader)
      2,                            // number of coordinates per vertex
      GL_FLOAT,                     // type
      GL_FALSE,                     // normalized?
      sizeof(math::Vec2f),          // stride
      (void *)(textureCoordsOffset) // array buffer offset
      );

  // request storage, but provide no data
  // [ vertices| uvs | normals ]
  glBufferData(GL_ARRAY_BUFFER, totalSize, NULL, GL_STATIC_DRAW);

  // load positions
  // [ v | v | v | ... | *uninitialized* ]
  glBufferSubData(GL_ARRAY_BUFFER,       // type
                  verticesOffset,        // offset
                  vertexSize,            // size
                  data.vertices.data()); // data pointer

  // load normals
  // [ v | v | v | .. | n | n | n | ... | *unitialized* ]
  glBufferSubData(GL_ARRAY_BUFFER,      // type
                  normalsOffset,        // offset
                  normalsSize,          // size
                  data.normals.data()); // data pointer

  // load uvs
  // [ v | v | v | .. | n | n | n | ... | uv | uv | uv ]
  glBufferSubData(GL_ARRAY_BUFFER,            // type
                  textureCoordsOffset,        // offset
                  textureCoordSize,           // size
                  data.textureCoords.data()); // data pointer

  vao.unbind();
  indexBuffer.unbind();
  vertexBuffer.unbind();

  return data.indices.size();
}

} // namespace opengl
