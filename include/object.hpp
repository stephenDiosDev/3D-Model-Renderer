#pragma once

#include "glad/glad.h"

#include <utility>

namespace opengl {

template <typename> void release_object(GLuint &name);

template <typename T> class Object final {
public:
  enum : GLuint { INVALID = 0 };

public:
  explicit Object(GLuint name) : m_name(name) {}
  ~Object() { release(); }

  // remove copy constructor/assignment
  Object(Object const &) = delete;
  Object &operator=(Object const &) = delete;

  // Move semantics
  Object(Object &&other) : m_name(other.m_name) { other.m_name = 0; }
  Object &operator=(Object &&other) {
    if (this != &other) {
      release();
      std::swap(m_name, other.m_name);
    }
    return *this;
  }

  GLuint name() const { return m_name; }

  bool isValid() const { return m_name != INVALID; }
  explicit operator bool() const { return isValid(); }

private:
  void release() {
    if (isValid()) {
      release_object<T>(m_name);
      m_name = INVALID;
    }
  }

private:
  GLuint m_name = INVALID;
};

} // namespace opengl
