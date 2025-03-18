#pragma once

#include <cstddef>

namespace ct {

template <class T>
class Matrix {
public:
  using ValueType = T;

  using Reference = T&;
  using ConstReference = const T&;

  using Pointer = T*;
  using ConstPointer = const T*;

  using Iterator = void;
  using ConstIterator = void;

  using RowIterator = void;
  using ConstRowIterator = void;

  using ColIterator = void;
  using ConstColIterator = void;

public:
  Matrix();

  Matrix(size_t rows, size_t cols);

  template <size_t ROWS, size_t COLS>
  Matrix(const T (&init)[ROWS][COLS]);

  Matrix(const Matrix& other);

  Matrix& operator=(const Matrix& other);

  ~Matrix();

  // Iterators

  Iterator begin();
  ConstIterator begin() const;
  Iterator end();
  ConstIterator end() const;

  RowIterator row_begin(size_t row);
  ConstRowIterator row_begin(size_t row) const;
  RowIterator row_end(size_t row);
  ConstRowIterator row_end(size_t row) const;

  ColIterator col_begin(size_t col);
  ConstColIterator col_begin(size_t col) const;
  ColIterator col_end(size_t col);
  ConstColIterator col_end(size_t col) const;

  // Size

  size_t rows() const;
  size_t cols() const;
  size_t size() const;
  bool empty() const;

  // Elements access

  Reference operator()(size_t row, size_t col);
  ConstReference operator()(size_t row, size_t col) const;

  Pointer data();
  ConstPointer data() const;

  // Comparison

  friend bool operator==(const Matrix& left, const Matrix& right);
  friend bool operator!=(const Matrix& left, const Matrix& right);

  // Arithmetic operations

  Matrix& operator+=(const Matrix& other);
  Matrix& operator-=(const Matrix& other);
  Matrix& operator*=(const Matrix& other);
  Matrix& operator*=(ConstReference factor);

  friend Matrix operator+(const Matrix& left, const Matrix& right);
  friend Matrix operator-(const Matrix& left, const Matrix& right);
  friend Matrix operator*(const Matrix& left, const Matrix& right);
  friend Matrix operator*(const Matrix& left, ConstReference right);
  friend Matrix operator*(ConstReference left, const Matrix& right);
};

} // namespace ct
