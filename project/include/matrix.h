#pragma once  // NOLINT

#include <istream>
#include <vector>

namespace prep {
class Matrix {
 public:
  explicit Matrix(size_t rows = 0, size_t cols = 0);
  explicit Matrix(std::istream& is);
  Matrix(const Matrix& rhs) = default;
  Matrix& operator=(const Matrix& rhs) = default;
  ~Matrix() = default;

  size_t getRows() const;
  size_t getCols() const;

  double operator()(size_t i, size_t j) const;
  double& operator()(size_t i, size_t j);

  bool operator==(const Matrix& rhs) const;
  bool operator!=(const Matrix& rhs) const;

  Matrix operator+(const Matrix& rhs) const;
  Matrix operator-(const Matrix& rhs) const;
  Matrix operator*(const Matrix& rhs) const;

  Matrix operator*(double val) const;

  friend Matrix operator*(double val, const Matrix& matrix);
  friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);

  Matrix transp() const;
  double det() const;
  Matrix adj() const;
  Matrix inv() const;

 private:
  size_t rows;
  size_t cols;
  std::vector<std::vector<double>> data;

  void det_calc(const Matrix& matrix, double* val) const;
  Matrix del_col_row(size_t row, size_t col) const;
  void free_matrix();
};

Matrix operator*(double val, const Matrix& matrix);
std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
}  // namespace prep
