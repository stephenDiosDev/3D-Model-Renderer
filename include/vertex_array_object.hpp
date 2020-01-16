#pragma once

#include <glad/glad.h>

#include "object.hpp"

namespace opengl {

class VertexArrayObject final {
public:
  bool isValid() const;
  explicit operator bool() const;

  void bind() const;
  void unbind() const;
  GLuint id() const;

private:
  /* Only called through makeVertexAttributeObject() factory function */
  VertexArrayObject(GLuint id);

  friend VertexArrayObject makeVertexArrayObject();
  friend void swap(VertexArrayObject &lhs, VertexArrayObject &rhs);

private:
  opengl::Object<VertexArrayObject> m_id;
};

VertexArrayObject makeVertexArrayObject();

void swap(VertexArrayObject &lhs, VertexArrayObject &rhs);

} // namespace openGL
