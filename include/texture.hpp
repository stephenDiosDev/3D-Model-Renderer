#pragma once

#include <utility>

#include "glad/glad.h"

#include "object.hpp"

namespace opengl {

class Texture {
public:
  Texture(GLuint name) : m_object(name) {}

  GLuint id() const;

  void bind() const;
  void unbind() const;

private:
  Object<Texture> m_object;
};

Texture generateTexture();

} // opengl
