#include "program.hpp"

#include <iostream>
#include <vector>
#include <cassert>

namespace opengl {

Program::Program(GLuint programID) : m_object(programID) {}

void Program::use() const { glUseProgram(id()); }

GLuint Program::id() const { return m_object.name(); }

bool Program::isValid() const { return id() != Program::INVALID_ID; }

Program::operator bool() const { return isValid(); }

bool isValidLocation(GLint location) { return location != -1; }

GLint Program::uniformLocation(const std::string &name) const {
  GLint location = glGetUniformLocation(id(), name.c_str());
  if (!isValidLocation(location)) {
	  std::cerr << "[ERROR] Invalid shader location: " << name.c_str() << '\n';
  }
  return location;
}

GLint Program::uniformLocation(GLchar const *name) const {
  GLint location = glGetUniformLocation(id(), name);
  if (!isValidLocation(location)) {
	  std::cerr << "[ERROR] Invalid shader location: " << name << '\n';
  }
  return location;
}

Program makeProgram(std::string const &vertexShaderSource,
                    std::string const &fragmentShaderSource) {
  auto vs = makeShader(vertexShaderSource, Shader::VERTEX);
  if (!vs.isValid()) {
    std::cerr << "Failed to compile vertex shader\n";
    return Program(Program::INVALID_ID);
  }
  auto fs = makeShader(fragmentShaderSource, Shader::FRAGMENT);
  if (!fs.isValid()) {
    std::cerr << "Failed to compile fragment shader\n";
    return Program(Program::INVALID_ID);
  }
  return makeProgram(vs, fs);
}

Program makeProgram(std::string const &vertexShaderSource,
                    std::string const &geometryShaderSource,
                    std::string const &fragmentShaderSource) {
  auto vs = makeShader(vertexShaderSource, Shader::VERTEX);
  if (!vs.isValid()) {
    std::cerr << "Failed to compile vertex shader\n";
    return Program(Program::INVALID_ID);
  }
  auto gs = makeShader(geometryShaderSource, Shader::GEOMETRY);
  if (!gs.isValid()) {
    std::cerr << "Failed to compile geometry shader\n";
    return Program(Program::INVALID_ID);
  }
  auto fs = makeShader(fragmentShaderSource, Shader::FRAGMENT);
  if (!fs.isValid()) {
    std::cerr << "Failed to compile fragment shader\n";
    return Program(Program::INVALID_ID);
  }
  return makeProgram(vs, gs, fs);
}

Program makeProgram(Shader const &vertexShader, Shader const &fragmentShader) {
  if (!vertexShader.isValid() || !fragmentShader.isValid()) {
    return Program(Program::INVALID_ID);
  }

  if ((vertexShader.type() != Shader::VERTEX) ||
      (fragmentShader.type() != Shader::FRAGMENT)) {
    return Program(Program::INVALID_ID);
  }

  Program program(glCreateProgram());
  if (!program) {
    return Program(Program::INVALID_ID);
  }

  glAttachShader(program.id(), vertexShader.id());
  glAttachShader(program.id(), fragmentShader.id());

  glLinkProgram(program.id());

  // detach so shaders may be deallocated properly
  glDetachShader(program.id(), vertexShader.id());
  glDetachShader(program.id(), fragmentShader.id());

  if (!checkLinkStatus(program.id())) {
    return Program(Program::INVALID_ID);
  }

  return program;
}

Program makeProgram(Shader const &vertexShader, Shader const &geometryShader,
                    Shader const &fragmentShader) {
  if (!vertexShader.isValid() || !fragmentShader.isValid() ||
      !geometryShader.isValid()) {
    return Program(Program::INVALID_ID);
  }

  if ((vertexShader.type() != Shader::VERTEX) ||
      (geometryShader.type() != Shader::GEOMETRY) ||
      (fragmentShader.type() != Shader::FRAGMENT)) {
    return Program(Program::INVALID_ID);
  }

  Program program(glCreateProgram());
  if (!program) {
    return Program(Program::INVALID_ID);
  }

  glAttachShader(program.id(), vertexShader.id());
  glAttachShader(program.id(), geometryShader.id());
  glAttachShader(program.id(), fragmentShader.id());

  glLinkProgram(program.id());

  glDetachShader(program.id(), vertexShader.id());
  glDetachShader(program.id(), geometryShader.id());
  glDetachShader(program.id(), fragmentShader.id());

  if (!checkLinkStatus(program.id())) {
    return Program(Program::INVALID_ID);
  }

  return program;
}

bool isValidProgramID(GLuint id) { return id != Program::INVALID_ID; }

bool checkLinkStatus(GLuint programID) {
  GLint result;
  int infoLogLength;
  glGetProgramiv(programID, GL_LINK_STATUS, &result);
  if (!result) {
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> errorMsg(infoLogLength);
    glGetProgramInfoLog(programID, infoLogLength, nullptr, errorMsg.data());
    std::cerr << errorMsg.data() << std::endl;
    return false;
  }

  return true;
}

void setUniformVec3f(GLuint uniformLocation, float x, float y, float z) {
  glUniform3f(uniformLocation, x, y, z);
}

void setUniformVec3f(GLuint uniformLocation, math::Vec3f const &vec) {
  glUniform3fv(uniformLocation, 1, vec.data());
}

void setUniformVec3f(GLuint uniformLocation, GLuint count,
                     float const *vecPtr) {
  glUniform3fv(uniformLocation, count, vecPtr);
}

void setUniformMat4f(GLuint uniformLocation, math::Mat4f const &value,
                     GLboolean applyTranspose) {
  glUniformMatrix4fv(uniformLocation, 1, applyTranspose, value.data());
}

void setUniform1f(GLuint uniformLocation, float value) {
  glUniform1f(uniformLocation, value);
}

template <> void release_object<Program>(GLuint &name) {
  glDeleteProgram(name);
}

} // namespace openGL
