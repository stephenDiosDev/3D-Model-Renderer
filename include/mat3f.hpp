#pragma once

#include <array>
#include <initializer_list>
#include <iosfwd>

namespace math {

class Mat3f {
public:
  enum { DIMENSION = 3, NUMBER_ELEMENTS = 9 };

  using array9f = std::array<float, NUMBER_ELEMENTS>;

public:
  Mat3f() = default;
  explicit Mat3f(float fillValue);
  explicit Mat3f(array9f values);
  Mat3f(std::initializer_list<float> list);

  void fill(float t);

  float &operator()(int row, int column);
  float &operator[](int element);
  float &at(int row, int column);
  float &at(int element);
  float *data();

  float operator()(int row, int column) const;
  float operator[](int element) const;
  float at(int row, int column) const;
  float at(int element) const;
  float const *data() const;

  Mat3f::array9f::iterator begin();
  Mat3f::array9f::iterator end();
  Mat3f::array9f::const_iterator begin() const;
  Mat3f::array9f::const_iterator end() const;

  static int rowMajorIndex(int row, int column);
  static Mat3f identity();

private:
  array9f m_values;
};

Mat3f transposed(Mat3f m);
float determinant(Mat3f const &m);
Mat3f inverse(Mat3f const &m);

Mat3f operator+(Mat3f const &lhs, Mat3f const &rhs);
Mat3f operator-(Mat3f const &lhs, Mat3f const &rhs);
Mat3f operator*(Mat3f const &lhs, Mat3f const &rhs);
Mat3f operator*(float s, Mat3f const &rhs);
Mat3f operator*(Mat3f const &lhs, float s);

std::ostream &operator<<(std::ostream &out, Mat3f const &mat);

} // namespace math
