#include "obj_mesh.hpp"

#include <algorithm>
#include <cassert>



using namespace math;

namespace geometry {

std::vector<math::Vec3f>
calculateTriangleNormals(std::vector<IndicesTriangle> const &indexTriangles,
                         std::vector<math::Vec3f> const &vertices) {
  std::vector<Vec3f> tmp;

  //vertices holds the list of all vertices from the mesh
  //indexTriangles holds the indices of every triangle, we need the vertexID so we can do"
  //vertices[vertexID]
  Vec3f AB;
  Vec3f AC;
//indexTriangles [spot in indexTriangles list][a, b, or c point of triangle][vertex, texture or normal]
  for(auto indTri : indexTriangles)
  {
      unsigned int vIDA = indTri[0][0];
      unsigned int vIDB = indTri[1][0];
      unsigned int vIDC = indTri[2][0];


      AB = vertices[vIDB] - vertices[vIDA];
      AC = vertices[vIDC] - vertices[vIDA];

      Vec3f normal = normalized(AB^AC);
      tmp.push_back(normal);
  }

  return tmp;
}

std::vector<math::Vec3f>
calculateVertexNormals(std::vector<IndicesTriangle> const &indexTriangles,
                       std::vector<math::Vec3f> const &vertices) {

    std::vector<Vec3f> tmp(indexTriangles.size());
    std::vector<Vec3f> triNormals = calculateTriangleNormals(indexTriangles, vertices);

    //needed to go through triNormals
    unsigned int index = 0;

    for(auto j : indexTriangles) {
        unsigned int vIDA = j[0][0];
        unsigned int vIDB = j[1][0];
        unsigned int vIDC = j[2][0];

        //adding 2 normalized
        tmp[vIDA] = normalized(tmp[vIDA] + triNormals[index]);
        tmp[vIDB] = normalized(tmp[vIDB] + triNormals[index]);
        tmp[vIDC] = normalized(tmp[vIDC] + triNormals[index]);

        index++;
    }

	return tmp;
}

std::vector<math::Vec3f>
calculateVertexNormals(std::vector<IndicesTriangle> const &indexTriangles,
                       std::vector<math::Vec3f> const &vertices,
                       std::vector<math::Vec3f> const &triangleNormals) {
    std::vector<Vec3f> tmp(indexTriangles.size());

    //needed to go through triNormals
    unsigned int index = 0;

    for(auto j : indexTriangles) {
        unsigned int vIDA = j[0][0];
        unsigned int vIDB = j[1][0];
        unsigned int vIDC = j[2][0];

        //adding 2 normalized
        tmp[vIDA] = normalized(tmp[vIDA] + triangleNormals[index]);
        tmp[vIDB] = normalized(tmp[vIDB] + triangleNormals[index]);
        tmp[vIDC] = normalized(tmp[vIDC] + triangleNormals[index]);

        index++;
    }

	return tmp;
}

} // namespace geometry
