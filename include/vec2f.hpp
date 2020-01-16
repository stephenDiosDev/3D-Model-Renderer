#pragma once

#include <iosfwd>

namespace math {

// struct declaration
struct Vec2f {
  float x = 0.f;
  float y = 0.f;

  Vec2f() = default;
  Vec2f(float x, float y);

  /*
   * Mutating member functions
   */
  Vec2f &operator+=(Vec2f const &rhs);
  Vec2f &operator-=(Vec2f const &rhs);
  Vec2f &operator*=(float rhs);
  Vec2f &operator/=(float rhs);

  Vec2f &operator=(Vec2f const &rhs) = default;

  Vec2f &normalize();

  float const *data() const;
  float *data();

  void zero();
};

// Free function declarations
Vec2f operator+(Vec2f const &a, Vec2f const &b);
Vec2f operator-(Vec2f const &a, Vec2f const &b);
Vec2f operator*(float s, Vec2f v);
Vec2f operator*(Vec2f v, float s);
Vec2f operator/(Vec2f v, float s);
Vec2f operator-(Vec2f v);

float operator*(Vec2f const &a, Vec2f const &b);
float dot(Vec2f const &a, Vec2f const &b);

float norm(Vec2f const &v);
float normSquared(Vec2f const &v);
Vec2f normalized(Vec2f v);

// Linear interpolation from a to b by t
Vec2f lerp(Vec2f const &a, Vec2f const &b, float t);
float distance(Vec2f const &a, Vec2f const &b);
float distanceSquared(Vec2f const &a, Vec2f const &b);

std::istream &operator>>(std::istream &in, Vec2f &v);
std::ostream &operator<<(std::ostream &out, Vec2f const &v);

} // namespace math
