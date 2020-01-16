#include "shader.hpp"

#include <iostream>
#include <vector>

namespace opengl {

Shader::Shader(GLuint id, Shader::Type type)
    : m_object(id), m_shaderType(type) {}

GLuint Shader::id() const { return m_object.name(); }

bool Shader::isValid() const {
  return isValidShaderID(m_object.name()) && isValidShaderType(m_shaderType);
}

Shader::operator bool() const { return isValid(); }

Shader::Type Shader::type() const { return m_shaderType; }

void swap(Shader &lhs, Shader &rhs) {
  std::swap(lhs.m_object, rhs.m_object);
  std::swap(lhs.m_shaderType, rhs.m_shaderType);
}

Shader::Type enumToShaderType(GLenum shaderTypeEnum) {
  switch (shaderTypeEnum) {
  case Shader::VERTEX:
  case Shader::GEOMETRY:
  case Shader::FRAGMENT:
    return static_cast<Shader::Type>(shaderTypeEnum);
  default:
    return Shader::INVALID;
  }
}

bool checkCompileStatus(GLuint shaderID) {
  GLint result;
  int infoLogLength;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
  if (!result) {
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> errorMsg(infoLogLength);
    glGetShaderInfoLog(shaderID, infoLogLength, nullptr, errorMsg.data());
    std::cerr << errorMsg.data() << std::endl;
    return false;
  }

  return true; // otherwise
}

Shader makeShader(std::string const &shaderCode, Shader::Type shaderTypeEnum) {
  auto type = enumToShaderType(shaderTypeEnum);
  if (!isValidShaderType(type)) {
    return Shader(Shader::INVALID_ID, Shader::INVALID);
  }

  Shader shader(glCreateShader(type), type);

  if (!shader) {
    return Shader(Shader::INVALID_ID, Shader::INVALID);
  }

  {
    char const *source = shaderCode.c_str();
    glShaderSource(shader.id(), 1, &source, nullptr);
  }

  glCompileShader(shader.id());

  if (!checkCompileStatus(shader.id())) {
    return Shader(Shader::INVALID_ID, Shader::INVALID);
  }

  return shader;
}

bool isValidShaderID(GLuint id) { return id != Shader::INVALID_ID; }

bool isValidShaderType(Shader::Type type) { return type != Shader::INVALID; }

template <> void release_object<Shader>(GLuint &name) { glDeleteShader(name); }

} // namespace openGL
