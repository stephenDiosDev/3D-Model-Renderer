#pragma once

#include <array>
#include <initializer_list>
#include <iosfwd>

namespace math {

class Mat4f {
public:
  enum { DIMENSION = 4, NUMBER_ELEMENTS = 16 };

  using array16f = std::array<float, NUMBER_ELEMENTS>;

public:
  Mat4f() = default;
  explicit Mat4f(float fillValue);
  explicit Mat4f(array16f values);
  Mat4f(std::initializer_list<float> list);

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

  Mat4f::array16f::iterator begin();
  Mat4f::array16f::iterator end();
  Mat4f::array16f::const_iterator begin() const;
  Mat4f::array16f::const_iterator end() const;

  static int rowMajorIndex(int row, int column);
  static Mat4f identity();

private:
  array16f m_values;
};

Mat4f transposed(Mat4f m);

Mat4f operator+(Mat4f const &lhs, Mat4f const &rhs);
Mat4f operator-(Mat4f const &lhs, Mat4f const &rhs);
Mat4f operator*(Mat4f const &lhs, Mat4f const &rhs);
Mat4f operator*(float s, Mat4f const &rhs);
Mat4f operator*(Mat4f const &lhs, float s);

std::ostream &operator<<(std::ostream &out, Mat4f const &mat);

} // namespace math
