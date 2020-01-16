#pragma once

#include <iosfwd>

namespace math {

// struct declaration
struct Vec3f {
  float x = 0.f;
  float y = 0.f;
  float z = 0.f;

  Vec3f() = default;
  Vec3f(float x, float y, float z);

  /*
   * Mutating member functions
   */
  Vec3f &operator+=(Vec3f const &rhs);
  Vec3f &operator-=(Vec3f const &rhs);
  Vec3f &operator*=(float rhs);
  Vec3f &operator/=(float rhs);

  Vec3f &operator=(Vec3f const &rhs) = default;

  Vec3f &normalize();

  float const *data() const;
  float *data();

  void zero();
};

// Free function declarations
Vec3f operator+(Vec3f const &a, Vec3f const &b);
Vec3f operator-(Vec3f const &a, Vec3f const &b);
Vec3f operator*(float s, Vec3f v);
Vec3f operator*(Vec3f v, float s);
Vec3f operator/(Vec3f v, float s);
Vec3f operator-(Vec3f v);

float operator*(Vec3f const &a, Vec3f const &b);
float dot(Vec3f const &a, Vec3f const &b);

Vec3f operator^(Vec3f const &a, Vec3f const &b);
Vec3f cross(Vec3f const &a, Vec3f const &b);

float norm(Vec3f const &v);
float normSquared(Vec3f const &v);
Vec3f normalized(Vec3f v);

Vec3f rotateAroundAxis(Vec3f v, Vec3f axis, float angleDegrees);

Vec3f rotateAroundNormalizedAxis(Vec3f v, Vec3f axis, float angleDegrees);

// Linear interpolation from a to b by t
Vec3f lerp(Vec3f const &a, Vec3f const &b, float t);
float distance(Vec3f const &a, Vec3f const &b);
float distanceSquared(Vec3f const &a, Vec3f const &b);

std::istream &operator>>(std::istream &in, Vec3f &v);
std::ostream &operator<<(std::ostream &out, Vec3f const &v);

} // namespace math
