#include "mat3f.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <functional>
#include <iostream>

namespace math {

Mat3f::Mat3f(float fillValue) { m_values.fill(fillValue); }

Mat3f::Mat3f(Mat3f::array9f values) : m_values(values) {}

Mat3f::Mat3f(std::initializer_list<float> list) {
  assert(list.size() == NUMBER_ELEMENTS);
  std::copy_n(list.begin(), NUMBER_ELEMENTS, m_values.begin());
}

void Mat3f::fill(float t) { m_values.fill(t); }

float &Mat3f::operator()(int row, int column) {
  return m_values[rowMajorIndex(row, column)];
}

float &Mat3f::operator[](int element) { return m_values[element]; }

float &Mat3f::at(int row, int column) {
  return m_values.at(rowMajorIndex(row, column));
}

float &Mat3f::at(int element) { return m_values.at(element); }

float *Mat3f::data() { return m_values.data(); }

float Mat3f::operator()(int row, int column) const {
  return m_values[rowMajorIndex(row, column)];
}

float Mat3f::operator[](int element) const { return m_values[element]; }

float Mat3f::at(int row, int column) const {
  return m_values.at(rowMajorIndex(row, column));
}

float Mat3f::at(int element) const { return m_values.at(element); }

float const *Mat3f::data() const { return m_values.data(); }

int Mat3f::rowMajorIndex(int row, int column) {
  return row * DIMENSION + column;
}

Mat3f Mat3f::identity() {
  return {1.f, 0.f, 0.f, //
          0.f, 1.f, 0.f, //
          0.f, 0.f, 1.f, //
          0.f, 0.f, 0.f};
}

Mat3f transposed(Mat3f mat) {
  using std::swap;
  //	0	1	2
  // --------------
  // 0|	0	1	2
  // 1| 3	4	5
  // 2| 6	7	8

  swap(mat[1], mat[3]);
  swap(mat[2], mat[6]);
  swap(mat[5], mat[7]);

  return mat;
}

float determinant(Mat3f const &m) {
  return m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) - //
         m(0, 1) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) + //
         m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0));
}

Mat3f inverse(Mat3f const &m) {
  Mat3f mInv;

  auto det = determinant(m);
  auto invDet = 1.f / det;

  mInv(0, 0) = (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) * invDet;
  mInv(0, 1) = (m(0, 2) * m(2, 1) - m(0, 1) * m(2, 2)) * invDet;
  mInv(0, 2) = (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) * invDet;
  mInv(1, 0) = (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) * invDet;
  mInv(1, 1) = (m(0, 0) * m(2, 2) - m(0, 2) * m(2, 0)) * invDet;
  mInv(1, 2) = (m(1, 0) * m(0, 2) - m(0, 0) * m(1, 2)) * invDet;
  mInv(2, 0) = (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)) * invDet;
  mInv(2, 1) = (m(2, 0) * m(0, 1) - m(0, 0) * m(2, 1)) * invDet;
  mInv(2, 2) = (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) * invDet;

  return mInv;
}

Mat3f operator+(Mat3f const &lhs, Mat3f rhs) {
  using std::begin;
  using std::end;

  std::transform(begin(lhs), end(lhs), begin(rhs), begin(rhs),
                 std::plus<float>());
  return rhs;
}

Mat3f operator*(Mat3f const &lhs, Mat3f const &rhs) {
  Mat3f result;

  float element = 0.f;
  for (int i = 0; i < Mat3f::DIMENSION; ++i) {
    for (int j = 0; j < Mat3f::DIMENSION; ++j) {
      element = 0.f;
      for (int k = 0; k < Mat3f::DIMENSION; ++k) {
        element += lhs(i, k) * rhs(k, j);
      }
      result(i, j) = element;
    }
  }

  return result;
}

Mat3f operator*(float s, Mat3f rhs) {
  using std::begin;
  using std::end;
  std::transform(begin(rhs), end(rhs), begin(rhs),
                 [=](float f) { return s * f; });
  return rhs;
}

Mat3f operator*(Mat3f lhs, float s) {
  using std::begin;
  using std::end;
  std::transform(begin(lhs), end(lhs), begin(lhs),
                 [=](float f) { return s * f; });
  return lhs;
}

Mat3f::array9f::iterator Mat3f::begin() { return m_values.begin(); }

Mat3f::array9f::iterator Mat3f::end() { return m_values.end(); }

Mat3f::array9f::const_iterator Mat3f::begin() const { return m_values.begin(); }

Mat3f::array9f::const_iterator Mat3f::end() const { return m_values.end(); }

std::ostream &operator<<(std::ostream &out, Mat3f const &mat) {
  using std::begin;
  using std::end;

  std::ostream_iterator<float> outIter(out, " ");
  std::copy(begin(mat), end(mat), outIter);

  return out;
}

} // namespace math
