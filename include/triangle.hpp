#pragma once

#include "vec3f.hpp"

namespace geometry {

template <typename T> class Triangle_ {
private:
  enum Index : unsigned char { A = 0, B, C, COUNT };

public:
  Triangle_() = default;
  Triangle_(T a, T b, T c);

  T const &a() const;
  T const &b() const;
  T const &c() const;

  T const &operator[](int index) const;

  T &a();
  T &b();
  T &c();

  T &operator[](int index);

private:
  T m_vertices[Index::COUNT];
};

using Triangle = Triangle_<math::Vec3f>;
using IndexTriangle = Triangle_<unsigned int>;

Triangle scale(Triangle t, float s);

Triangle translate(Triangle t, math::Vec3f const &v);

} // namespace geometry

#include "triangle.tpp"
