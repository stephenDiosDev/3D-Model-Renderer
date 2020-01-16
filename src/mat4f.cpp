#include "mat4f.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <functional>
#include <iostream>

namespace math {

Mat4f::Mat4f(float fillValue) { m_values.fill(fillValue); }

Mat4f::Mat4f(Mat4f::array16f values) : m_values(values) {}

Mat4f::Mat4f(std::initializer_list<float> list) {
  assert(list.size() == NUMBER_ELEMENTS);
  std::copy_n(list.begin(), NUMBER_ELEMENTS, m_values.begin());
}

void Mat4f::fill(float t) { m_values.fill(t); }

float &Mat4f::operator()(int row, int column) {
  return m_values[rowMajorIndex(row, column)];
}

float &Mat4f::operator[](int element) { return m_values[element]; }

float &Mat4f::at(int row, int column) {
  return m_values.at(rowMajorIndex(row, column));
}

float &Mat4f::at(int element) { return m_values.at(element); }

float *Mat4f::data() { return m_values.data(); }

float Mat4f::operator()(int row, int column) const {
  return m_values[rowMajorIndex(row, column)];
}

float Mat4f::operator[](int element) const { return m_values[element]; }

float Mat4f::at(int row, int column) const {
  return m_values.at(rowMajorIndex(row, column));
}

float Mat4f::at(int element) const { return m_values.at(element); }

float const *Mat4f::data() const { return m_values.data(); }

int Mat4f::rowMajorIndex(int row, int column) {
  return row * DIMENSION + column;
}

Mat4f Mat4f::identity() {
  return {1.f, 0.f, 0.f, 0.f, // row 0
          0.f, 1.f, 0.f, 0.f, // row 1
          0.f, 0.f, 1.f, 0.f, // row 2
          0.f, 0.f, 0.f, 1.f};
}

Mat4f transposed(Mat4f mat) {
  using std::swap;
  //		0		1		2		3
  // ----------------
  // 0|	0		1		2		3
  // 1| 4		5		6		7
  // 2| 8		9		10	11
  // 3| 12	13	14	15

  swap(mat[1], mat[4]);
  swap(mat[2], mat[8]);
  swap(mat[3], mat[12]);
  swap(mat[6], mat[9]);
  swap(mat[7], mat[13]);
  swap(mat[11], mat[14]);

  return mat;
}

Mat4f operator+(Mat4f const &lhs, Mat4f rhs) {
  using std::begin;
  using std::end;

  std::transform(begin(lhs), end(lhs), begin(rhs), begin(rhs),
                 std::plus<float>());
  return rhs;
}

Mat4f operator*(Mat4f const &lhs, Mat4f const &rhs) {
  Mat4f result;

  float element = 0.f;
  for (int i = 0; i < Mat4f::DIMENSION; ++i) {
    for (int j = 0; j < Mat4f::DIMENSION; ++j) {
      element = 0.f;
      for (int k = 0; k < Mat4f::DIMENSION; ++k) {
        element += lhs(i, k) * rhs(k, j);
      }
      result(i, j) = element;
    }
  }

  return result;
}

Mat4f operator*(float s, Mat4f rhs) {
  using std::begin;
  using std::end;
  std::transform(begin(rhs), end(rhs), begin(rhs),
                 [=](float f) { return s * f; });
  return rhs;
}

Mat4f operator*(Mat4f lhs, float s) {
  using std::begin;
  using std::end;
  std::transform(begin(lhs), end(lhs), begin(lhs),
                 [=](float f) { return s * f; });
  return lhs;
}

Mat4f::array16f::iterator Mat4f::begin() { return m_values.begin(); }

Mat4f::array16f::iterator Mat4f::end() { return m_values.end(); }

Mat4f::array16f::const_iterator Mat4f::begin() const {
  return m_values.begin();
}

Mat4f::array16f::const_iterator Mat4f::end() const { return m_values.end(); }

std::ostream &operator<<(std::ostream &out, Mat4f const &mat) {
  using std::begin;
  using std::end;

  std::ostream_iterator<float> outIter(out, " ");
  std::copy(begin(mat), end(mat), outIter);

  return out;
}

} // namespace math
