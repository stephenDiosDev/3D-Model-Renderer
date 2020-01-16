#pragma once

#include "mat4f.hpp"
#include "mat3f.hpp"
#include "vec3f.hpp"

namespace math {

math::Mat4f uniformScaleMatrix(float scale);

math::Mat4f scaleMatrix(float x, float y, float z);

math::Mat4f scaleMatrix(math::Vec3f const &scale);

math::Mat4f translateMatrix(float x, float y, float z);

math::Mat4f translateMatrix(math::Vec3f const &pos);

math::Mat4f rotateAboutXMatrix(float angleDeg);

math::Mat4f rotateAboutYMatrix(float angleDeg);

math::Mat4f rotateAboutZMatrix(float angleDeg);

math::Mat4f orthographicProjection(float left, float right, float top,
                                   float bottom, float near, float far);

math::Mat4f symmetricOrthographicProjection(float right, float top, float near,
                                            float far);

math::Mat4f frustumProjection(float left, float right, float top, float bottom,
                              float near, float far);

math::Mat4f inverseFrustumProjection(float left, float right, float top,
                                     float bottom, float near, float far);

math::Mat4f symmetricFrustumProjection(float right, float top, float near,
                                       float far);

math::Mat4f inverseSymmetricFrustumProjection(float right, float top,
                                              float near, float far);

math::Mat4f perspectiveProjection(float fovDegrees, float aspectRatio,
                                  float zNear, float zFar);

math::Mat4f inversePerspectiveProjection(float fovDegrees, float aspectRaito,
                                         float zNear, float zFar);

math::Mat4f lookAtMatrix(const math::Vec3f &pos, const math::Vec3f &target,
                         const math::Vec3f &up);

math::Mat4f inverseLookAtMatrix(const math::Vec3f &pos,
                                const math::Vec3f &target,
                                const math::Vec3f &up);

math::Mat3f mat3(Mat4f const &m);
	
math::Mat4f mat4(Mat3f const &m);

} // namespace math
