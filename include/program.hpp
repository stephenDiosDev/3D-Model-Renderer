#pragma once

#include <string>

#include <glad/glad.h>

#include "mat4f.hpp"
#include "shader.hpp"
#include "vec3f.hpp"
#include "object.hpp"

namespace opengl {

class Program final {
public:
  enum : GLuint { INVALID_ID = 0 };

  void use() const;
  bool isValid() const;

  explicit operator bool() const;

  GLuint id() const;

  GLint uniformLocation(std::string const &name) const;
  GLint uniformLocation(GLchar const *name) const;

private:
  /* Only called through makeProgram() factory function */
  explicit Program(GLuint programID);

  void release();

  friend Program makeProgram(std::string const &vertexShaderSource,
                             std::string const &fragmentShaderSource);

  friend Program makeProgram(std::string const &vertexShaderSource,
                             std::string const &geometryShaderSource,
                             std::string const &fragmentShaderSource);

  friend Program makeProgram(Shader const &vertexShader,
                             Shader const &fragmentShader);

  friend Program makeProgram(Shader const &vertexShader,
                             Shader const &geometryShader,
                             Shader const &fragmentShader);

private:
  opengl::Object<Program> m_object;
};

Program makeProgram(std::string const &vertexShaderSource,
                    std::string const &fragmentShaderSource);

Program makeProgram(std::string const &vertexShaderSource,
                    std::string const &geometryShaderSource,
                    std::string const &fragmentShaderSource);

Program makeProgram(Shader const &vertexShader, Shader const &fragmentShader);

Program makeProgram(Shader const &vertexShader, Shader const &geometryShader,
                    Shader const &fragmentShader);

bool isValidProgramID(GLuint id);

bool checkLinkStatus(GLuint programID);

void setUniformVec3f(GLuint uniformLocation, float x, float y, float z);
void setUniformVec3f(GLuint uniformLocation, math::Vec3f const &value);
void setUniformVec3f(GLuint uniformLocation, GLuint count, float const *vecPtr);

void setUniformMat4f(GLuint uniformLocation, math::Mat4f const &value,
                     GLboolean applyTranspose = GL_FALSE);

void setUniform1f(GLuint uniformLocation, float value);

} // namespace openGL
