#include "vec3f.hpp"

#include <cmath>
#include <iostream>

namespace math {

Vec3f::Vec3f(float x, float y, float z) : x(x), y(y), z(z) {}

Vec3f &Vec3f::operator+=(Vec3f const &rhs) {
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  return *this;
}
Vec3f &Vec3f::operator-=(Vec3f const &rhs) {
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  return *this;
}

Vec3f &Vec3f::operator*=(float rhs) {
  x *= rhs;
  y *= rhs;
  z *= rhs;
  return *this;
}

Vec3f &Vec3f::operator/=(float rhs) {
  x /= rhs;
  y /= rhs;
  z /= rhs;
  return *this;
}

Vec3f &Vec3f::normalize() {
  float l = norm(*this);
  return (*this) /= l;
}

void Vec3f::zero() {
  x = 0.f;
  y = 0.f;
  z = 0.f;
}

float const *Vec3f::data() const {
  return &x; // warning!!!!! this is quite dangerous
}

float *Vec3f::data() {
  return &x; // warning!!!!! this is quite dangerous
}

// Free function
/*
 * Vector-Vector Addition/ Subtraction
 */
Vec3f operator+(Vec3f const &a, Vec3f const &b) {
  return Vec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}
Vec3f operator-(Vec3f const &a, Vec3f const &b) {
  return Vec3f(a.x - b.x, a.y - b.y, a.z - b.z);
}

/*
 * Scalar-Vector Multiplication/Division
 */
Vec3f operator*(float s, Vec3f v) {
  v.x *= s;
  v.y *= s;
  v.z *= s;
  return v;
}
Vec3f operator*(Vec3f v, float s) { return s * v; }

Vec3f operator/(Vec3f v, float s) {
  v.x /= s;
  v.y /= s;
  v.z /= s;
  return v;
}

/*
 * Negation of vector
 * -v = (-1.f) * v
 */
Vec3f operator-(Vec3f v) {
  v.x = -v.x;
  v.y = -v.y;
  v.z = -v.z;
  return v;
}

/*
 * Vector-Vector (inner/dot) product
 */
float operator*(Vec3f const &a, Vec3f const &b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}
float dot(Vec3f const &a, Vec3f const &b) { return a * b; }

/*
 * Vector-Vector cross product
 */
Vec3f operator^(Vec3f const &a, Vec3f const &b) {
  return Vec3f(a.y * b.z - a.z * b.y, //
               a.z * b.x - a.x * b.z, //
               a.x * b.y - a.y * b.x);
} Vec3f cross(Vec3f const &a, Vec3f const &b) {
  return a ^ b;
}

/*
 * Vector norm (length)
 */
float norm(Vec3f const &v) {
  return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
float normSquared(Vec3f const &v) { return v.x * v.x + v.y * v.y + v.z * v.z; }

/*
 * Normalized Vector
 */
Vec3f normalized(Vec3f v) {
  float l = norm(v);
  return v /= l;
}

Vec3f rotateAroundAxis(Vec3f v, Vec3f axis, float angleDegrees) {
  // Rodrigues formula
  // rotates a vector around an arbitrary axis by an angle (degrees)

  constexpr float degreesToRadians = M_PI / 180.f;
  float const sinTheta = std::sin(angleDegrees * degreesToRadians);
  float const cosTheta = std::cos(angleDegrees * degreesToRadians);

  axis.normalize();

  return v * cosTheta + (axis ^ v) * sinTheta +
         axis * ((axis * v) * (1.f - cosTheta));
}

Vec3f rotateAroundNormalizedAxis(Vec3f v, Vec3f const &axis,
                                 float angleDegrees) {
  // Rodrigues formula
  // rotates a vector around an arbitrary axis by an angle (degrees)

  constexpr float degreesToRadians = M_PI / 180.f;
  float const sinTheta = std::sin(angleDegrees * degreesToRadians);
  float const cosTheta = std::cos(angleDegrees * degreesToRadians);

  return v * cosTheta + (axis ^ v) * sinTheta +
         axis * ((axis * v) * (1.f - cosTheta));
}

float distance(Vec3f const &a, Vec3f const &b) { return norm(a - b); }

float distanceSquared(Vec3f const &a, Vec3f const &b) {
  return normSquared(a - b);
}

/*
 * Linear interpolation
 */
Vec3f lerp(Vec3f const &a, Vec3f const &b, float t) {
  return (1.f - t) * a + t * b;
}

std::ostream &operator<<(std::ostream &out, Vec3f const &v) {
  return out << v.x << " " << v.y << " " << v.z;
}

std::istream &operator>>(std::istream &in, Vec3f &v) {
  return in >> v.x >> v.y >> v.z;
}

} // namespace math
