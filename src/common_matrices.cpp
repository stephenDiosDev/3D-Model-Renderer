#include "common_matrices.hpp"

#include <cmath>

namespace math {

Mat4f uniformScaleMatrix(float scale) {
  Mat4f uniform = Mat4f{
      scale, 0.f,   0.f,
      0.f, //
      0.f,   scale, 0.f,
      0.f, //
      0.f,   0.f,   scale,
      0.f, //
      0.f,   0.f,   0.f,
      1.f //
  };

  return uniform;
}

Mat4f scaleMatrix(float x, float y, float z) {
  Mat4f scale = {
      x,   0.f, 0.f,
      0.f, //
      0.f, y,   0.f,
      0.f, //
      0.f, 0.f, z,
      0.f, //
      0.f, 0.f, 0.f,
      1.f //
  };

  return scale;
}

Mat4f scaleMatrix(Vec3f const &s) {
  Mat4f scale = {
      s.x, 0.f, 0.f,
      0.f, //
      0.f, s.y, 0.f,
      0.f, //
      0.f, 0.f, s.z,
      0.f, //
      0.f, 0.f, 0.f,
      1.f //
  };

  return scale;
}

Mat4f translateMatrix(float x, float y, float z) {
  Mat4f trans = {
      1.f, 0.f, 0.f,
      x, //
      0.f, 1.f, 0.f,
      y, //
      0.f, 0.f, 1.f,
      z, //
      0.f, 0.f, 0.f,
      1.f //
  };

  return trans;
}

Mat4f translateMatrix(Vec3f const &pos) {
  Mat4f trans = {
      1.f,   0.f, 0.f,
      pos.x, // 1 0 0 x
      0.f,   1.f, 0.f,
      pos.y, // 0 1 0 y
      0.f,   0.f, 1.f,
      pos.z, // 0 0 1 z
      0.f,   0.f, 0.f,
      1.f // 0 0 0 1
  };

  return trans;
}

Mat4f rotateAboutXMatrix(float angleDeg) {
  float angleRad = angleDeg * (M_PI / 180.f);

  float c = std::cos(angleRad);
  float s = std::sin(angleRad);

  Mat4f rot = {
      1.f, 0.f, 0.f,
      0.f, //
      0.f, c,   -s,
      0.f, //
      0.f, s,   c,
      0.f, //
      0.f, 0.f, 0.f,
      1.f //
  };
  return rot;
}

Mat4f rotateAboutYMatrix(float angleDeg) {
  float angleRad = angleDeg * (M_PI / 180.f);

  float c = std::cos(angleRad);
  float s = std::sin(angleRad);

  Mat4f rot = {
      c,   0.f, s,
      0.f, //
      0.f, 1.f, 0.f,
      0.f, //
      -s,  0.f, c,
      0.f, //
      0.f, 0.f, 0.f,
      1.f //
  };

  return rot;
}

Mat4f rotateAboutZMatrix(float angleDeg) {
  float angleRad = angleDeg * (M_PI / 180.f);

  float c = std::cos(angleRad);
  float s = std::sin(angleRad);

  Mat4f rot = {
      c,   -s,  0.f,
      0.f, //
      s,   c,   0.f,
      0.f, //
      0.f, 0.f, 1.f,
      0.f, //
      0.f, 0.f, 0.f,
      1.f //
  };

  return rot;
}

Mat4f orthographicProjection(float l, float r, float t, float b, float n,
                             float f) {
  float a00 = 2.f / (r - l);
  float a03 = -(r + l) / (r - l);
  float a11 = 2.f / (t - b);
  float a13 = -(t + b) / (t - b);
  float a22 = -2.f / (f - n);
  float a23 = -(f + n) / (f - n);

  Mat4f ortho = {
      a00, 0.f, 0.f,
      a03, //
      0.f, a11, 0.f,
      a13, //
      0.f, 0.f, a22,
      a23, //
      0.f, 0.f, 0.f,
      1.f //
  };
  return ortho;
}

Mat4f symmetricOrthographicProjection(float r, float t, float n, float f) {
  float a00 = 1.f / r;
  float a11 = 1.f / t;
  float a22 = -2.f / (f - n);
  float a23 = -(f + n) / (f - n);

  Mat4f ortho = {
      a00, 0.f, 0.f,
      0.f, //
      0.f, a11, 0.f,
      0.f, //
      0.f, 0.f, a22,
      a23, //
      0.f, 0.f, 0.f,
      1.f //
  };
  return ortho;
}

Mat4f frustumProjection(float l, float r, float t, float b, float n, float f) {
  float a00 = (2.f * n) / (r - l);
  float a02 = (r + l) / (r - l);
  float a11 = (2.f * n) / (t - b);
  float a12 = (t + b) / (t - b);
  float a22 = -(f + n) / (f - n);
  float a23 = -2.f * f * n / (f - n);

  Mat4f frustum = {
      a00, 0.f, a02,
      0.f, // 0 1 2 3
      0.f, a11, a12,
      0.f, // 4 5 6 7
      0.f, 0.f, a22,
      a23, // 8 9 10 11
      0.f, 0.f, -1.f,
      0.f // 12 13 14 15
  };
  return frustum;
}

Mat4f inverseFrustumProjection(float l, float r, float t, float b, float n,
                               float f) {
  float a00 = (r - l) * (2.f * n);
  float a03 = (r + l) / (2.f * n);
  float a11 = (t - b) / (2.f * n);
  float a13 = (t + b) / (2.f * n);
  float a32 = -(f - n) / (2.f * f * n);
  float a33 = (f + n) / (2.f * f * n);

  Mat4f frustum = {a00, 0.f, 0.f, a03,  // 0 1 2 3
                   0.f, a11, 0.f, a13,  // 4 5 6 7
                   0.f, 0.f, 0.f, -1.f, // 8 9 10 11
                   0.f, 0.f, a32, a33}; // 12 13 14 15
  return frustum;
}

Mat4f symmetricFrustumProjection(float r, float t, float n, float f) {
  float a00 = n / r;
  float a11 = n / t;
  float a22 = -(f + n) / (f - n);
  float a23 = -2.f * f * n / (f - n);

  Mat4f symmetricFrustum = {a00, 0.f, 0.f,  0.f,  // 0 1 2 3
                            0.f, a11, 0.f,  0.f,  // 4 5 6 7
                            0.f, 0.f, a22,  a23,  // 8 9 10 11
                            0.f, 0.f, -1.f, 0.f}; // 12 13 14 15

  return symmetricFrustum;
}

Mat4f inverseSymmetricFrustumProjection(float r, float t, float n, float f) {
  float a00 = r / n;
  float a11 = t / n;
  float a32 = -(f - n) / (2.f * f * n);
  float a33 = (f + n) / (2.f * f * n);

  Mat4f invSymmetricFrustum = {a00, 0.f, 0.f, 0.f,  // 0 1 2 3
                               0.f, a11, 0.f, 0.f,  // 4 5 6 7
                               0.f, 0.f, a00, -1.f, // 8 9 10 11
                               0.f, 0.f, a32, a33}; // 12 13 14 15

  return invSymmetricFrustum;
}

Mat4f perspectiveProjection(float fovDegrees, float aspectRatio, float zNear,
                            float zFar) {
  float top = std::tan(fovDegrees * (M_PI / 180.f) * (1.f / 2.f)) * zNear;
  float right = top * aspectRatio;

  return symmetricFrustumProjection(right, top, zNear, zFar);
}

Mat4f inversePerspectiveProjection(float fovDegrees, float aspectRatio,
                                   float zNear, float zFar) {
  float top = std::tan(fovDegrees * (M_PI / 180.f) * (1.f / 2.f)) * zNear;
  float right = top * aspectRatio;

  return inverseSymmetricFrustumProjection(right, top, zNear, zFar);
}

Mat4f lookAtMatrix(const Vec3f &eye, const Vec3f &target, const Vec3f &up) {
  Vec3f w = eye - target; // inverted for R-handed CS
  w.normalize();
  Vec3f u = normalized(up);
  Vec3f r = normalized(u ^ w);
  u = normalized(w ^ r);

  Mat4f view = {r.x, r.y, r.z, -eye.x, //
                u.x, u.y, u.z, -eye.y, //
                w.x, w.y, w.z, -eye.z, //
                0.f, 0.f, 0.f, 1.f};
  return view;
}

Mat4f inverseLookAtMatrix(const Vec3f &eye, const Vec3f &target,
                          const Vec3f &up) {
  Vec3f w = eye - target; // inverted for R-handed CS
  w.normalize();
  Vec3f u = normalized(up);
  Vec3f r = normalized(u ^ w);
  u = normalized(w ^ r);

  Mat4f inverseView = {r.x, u.x, w.x, eye.x, //
                       r.y, u.y, w.y, eye.y, //
                       r.z, u.z, w.z, eye.z, //
                       0.f, 0.f, 0.f, 1.f};
  return inverseView;
}

math::Mat3f mat3(Mat4f const &m) {
  return {m(0, 0), m(0, 1), m(0, 2), //
          m(1, 0), m(1, 1), m(1, 2), //
          m(2, 0), m(2, 1), m(2, 2)};
}

math::Mat4f mat4(Mat3f const &m) {
  return {m(0, 0), m(0, 1), m(0, 2), 0, //
          m(1, 0), m(1, 1), m(1, 2), 0, //
          m(2, 0), m(2, 1), m(2, 2), 0, //
          0,       0,       0,       1};
}

} // namespace math
