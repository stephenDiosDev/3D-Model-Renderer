#include "buffer_object.hpp"

#include <cassert>
#include <utility>

namespace opengl {

BufferObject::BufferObject(GLuint id) : m_id(id) {}

BufferObject::Type BufferObject::type() const { return m_type; }

bool BufferObject::isAllocated() const { return m_id.isValid(); }

bool BufferObject::isValidType() const {
  return isValidBufferObjectType(m_type);
}

bool BufferObject::isValid() const { return isAllocated() && isValidType(); }

BufferObject::operator bool() const { return isValid(); }

void BufferObject::bind() const {
  assert(isValid());
  glBindBuffer(m_type, m_id.name());
}

void BufferObject::bind(BufferObject::Type type) {
  assert(isAllocated());
  assert(isValidBufferObjectType(enumToBufferObjectType(type)));
  glBindBuffer(type, m_id.name());
  m_type = type;
}

void BufferObject::unbind() {
  assert(isValid());
  glBindBuffer(m_type, 0);
}

GLuint BufferObject::id() const { return m_id.name(); }

/** FREE FUNCTIONS **/

BufferObject makeBufferObject() {
  GLuint id = Object<BufferObject>::INVALID;
  glGenBuffers(1, &id);
  return BufferObject(id);
}

void swap(BufferObject &lhs, BufferObject &rhs) {
  std::swap(lhs.m_id, rhs.m_id);
  std::swap(lhs.m_type, rhs.m_type);
}

bool isValidBufferObjectType(BufferObject::Type type) {
  return type != BufferObject::INVALID;
}

BufferObject::Type enumToBufferObjectType(GLenum bufferObjectEnum) {
  switch (bufferObjectEnum) {
  case BufferObject::ARRAY:
  case BufferObject::ELEMENT_ARRAY:
  case BufferObject::TEXTURE:
    return static_cast<BufferObject::Type>(bufferObjectEnum);
  default:
    return BufferObject::INVALID;
  }
}

template <> void release_object<BufferObject>(GLuint &name) {
  glDeleteBuffers(1, &name);
}

} // namespace opengl
