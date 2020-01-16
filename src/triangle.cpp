#include "triangle.hpp"

namespace geometry {

Triangle scale(Triangle t, float s) {
  t.a() *= s;
  t.b() *= s;
  t.c() *= s;

  return t;
}

Triangle translate(Triangle t, math::Vec3f const &v) {
  t.a() += v;
  t.b() += v;
  t.c() += v;

  return t;
}

} // namespace
