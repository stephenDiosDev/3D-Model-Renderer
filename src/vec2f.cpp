#include "vec2f.hpp"

#include <cmath>
#include <iostream>

namespace math {

Vec2f::Vec2f(float x, float y) : x(x), y(y) {}

Vec2f &Vec2f::operator+=(Vec2f const &rhs) {
  x += rhs.x;
  y += rhs.y;
  return *this;
}
Vec2f &Vec2f::operator-=(Vec2f const &rhs) {
  x -= rhs.x;
  y -= rhs.y;
  return *this;
}

Vec2f &Vec2f::operator*=(float rhs) {
  x *= rhs;
  y *= rhs;
  return *this;
}

Vec2f &Vec2f::operator/=(float rhs) {
  x /= rhs;
  y /= rhs;
  return *this;
}

Vec2f &Vec2f::normalize() {
  float l = norm(*this);
  return (*this) /= l;
}

void Vec2f::zero() {
  x = 0.f;
  y = 0.f;
}

float const *Vec2f::data() const {
  return &x; // warning!!!!! this is quite dangerous
}

float *Vec2f::data() {
  return &x; // warning!!!!! this is quite dangerous
}

// Free function
/*
 * Vector-Vector Addition/ Subtraction
 */
Vec2f operator+(Vec2f const &a, Vec2f const &b) {
  return Vec2f(a.x + b.x, a.y + b.y);
}
Vec2f operator-(Vec2f const &a, Vec2f const &b) {
  return Vec2f(a.x - b.x, a.y - b.y);
}

/*
 * Scalar-Vector Multiplication/Division
 */
Vec2f operator*(float s, Vec2f v) {
  v.x *= s;
  v.y *= s;
  return v;
}
Vec2f operator*(Vec2f v, float s) { return s * v; }

Vec2f operator/(Vec2f v, float s) {
  v.x /= s;
  v.y /= s;
  return v;
}

/*
 * Negation of vector
 * -v = (-1.f) * v
 */
Vec2f operator-(Vec2f v) {
  v.x = -v.x;
  v.y = -v.y;
  return v;
}

/*
 * Vector-Vector (inner/dot) product
 */
float operator*(Vec2f const &a, Vec2f const &b) {
  return a.x * b.x + a.y * b.y;
}
float dot(Vec2f const &a, Vec2f const &b) { return a * b; }

/*
 * Vector norm (length)
 */
float norm(Vec2f const &v) { return std::sqrt(v * v); }
float normSquared(Vec2f const &v) { return v * v; }

/*
 * Normalized Vector
 */
Vec2f normalized(Vec2f v) {
  float l = norm(v);
  return v /= l;
}

/*
 * Linear interpolation
 */
Vec2f lerp(Vec2f const &a, Vec2f const &b, float t) {
  return (1.f - t) * a + t * b;
}

std::ostream &operator<<(std::ostream &out, Vec2f const &v) {
  return out << v.x << " " << v.y;
}

std::istream &operator>>(std::istream &in, Vec2f &v) {
  return in >> v.x >> v.y;
}

} // namespace math
