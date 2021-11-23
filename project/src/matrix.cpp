#include "matrix.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

#include "exceptions.h"

namespace prep {
Matrix::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
  data.resize(rows);
  for (size_t i = 0; i < rows; i++) data[i].resize(cols);
}

Matrix::Matrix(std::istream& is) {
  if (!is) throw(InvalidMatrixStream());
  if (!(is >> rows)) throw(InvalidMatrixStream());

  if (!(is >> cols)) throw(InvalidMatrixStream());

  data.resize(rows);
  for (size_t i = 0; i < rows; i++) data[i].resize(cols);

  for (size_t i = 0; i < rows; i++)
    for (size_t j = 0; j < cols; j++)
      if (!(is >> data[i][j])) throw(InvalidMatrixStream());
}

size_t Matrix::getRows() const { return rows; }

size_t Matrix::getCols() const { return cols; }

double Matrix::operator()(size_t i, size_t j) const {
  if (i > rows - 1 || j > cols - 1) throw(OutOfRange(i, j, *this));

  return data[i][j];
}

double& Matrix::operator()(size_t i, size_t j) {
  if (i > rows - 1 || j > cols - 1) throw(OutOfRange(i, j, *this));

  return data[i][j];
}
bool Matrix::operator==(const Matrix& rhs) const {
  if (rows != rhs.getRows() || cols != rhs.getCols()) return false;

  for (size_t i = 0; i < rows; i++)
    for (size_t j = 0; j < cols; j++)
      if (fabs(data[i][j] - rhs(i, j)) > 1e-7) return false;
  return true;
}

bool Matrix::operator!=(const Matrix& rhs) const {
  if (rows != rhs.getRows() || cols != rhs.getCols()) return true;

  for (size_t i = 0; i < rows; i++)
    for (size_t j = 0; j < cols; j++)
      if (fabs(data[i][j] - rhs(i, j)) > 1e-7) return true;
  return false;
}

Matrix Matrix::operator+(const Matrix& rhs) const {  // DimensionMismatch
  if (rows != rhs.getRows() || cols != rhs.getCols())
    throw(DimensionMismatch(*this, rhs));

  Matrix matrix_sum(rows, cols);
  for (size_t i = 0; i < rows; i++)
    for (size_t j = 0; j < cols; j++) matrix_sum(i, j) = data[i][j] + rhs(i, j);

  return matrix_sum;
}

Matrix Matrix::operator-(const Matrix& rhs) const {  // DimensionMismatch
  if (rows != rhs.getRows() || cols != rhs.getCols())
    throw(DimensionMismatch(*this, rhs));

  Matrix matrix_sum(rows, cols);

  for (size_t i = 0; i < rows; i++)
    for (size_t j = 0; j < cols; j++) matrix_sum(i, j) = data[i][j] - rhs(i, j);

  return matrix_sum;
}

Matrix Matrix::operator*(const Matrix& rhs) const {  // DimensionMismatch
  if (cols != rhs.getRows()) throw(DimensionMismatch(*this, rhs));

  Matrix matrix_mul(rows, rhs.getCols());

  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < rhs.getCols(); j++) {
      matrix_mul(i, j) = 0;
      for (size_t k = 0; k < cols; k++)
        matrix_mul(i, j) += data[i][k] * rhs(k, j);
    }
  }

  return matrix_mul;
}

Matrix Matrix::operator*(double val) const {
  Matrix matrix_mul_scal(rows, cols);

  for (size_t i = 0; i < rows; i++)
    for (size_t j = 0; j < cols; j++) matrix_mul_scal(i, j) = data[i][j] * val;

  return matrix_mul_scal;
}

Matrix operator*(double val, const Matrix& matrix) {
  Matrix matrix_mul_scal(matrix.getRows(), matrix.getCols());

  for (size_t i = 0; i < matrix.getRows(); i++)
    for (size_t j = 0; j < matrix.getCols(); j++)
      matrix_mul_scal(i, j) = matrix(i, j) * val;

  return matrix_mul_scal;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
  if (!os) throw(InvalidMatrixStream());
  os << matrix.getRows() << ' ' << matrix.getCols() << std::endl;
  for (size_t i = 0; i < matrix.getRows(); i++) {
    for (size_t j = 0; j < matrix.getCols(); j++) {
      os << std::setprecision(std::numeric_limits<double>::max_digits10)
         << matrix.data[i][j];
      if (j < matrix.getCols() - 1) os << ' ';
    }
    os << std::endl;
  }

  return os;
}

Matrix Matrix::transp() const {
  Matrix new_matr(cols, rows);
  for (size_t i = 0; i < rows; i++)
    for (size_t j = 0; j < cols; j++) new_matr(j, i) = data[i][j];

  return new_matr;
}

Matrix Matrix::del_col_row(size_t row, size_t col) const {
  Matrix new_matr(rows - 1, cols - 1);
  size_t i_1 = 0;
  size_t j_1 = 0;
  for (size_t i = 0; i < rows; i++) {
    if (i != row) {
      for (size_t j = 0; j < cols; j++) {
        if (j != col) {
          new_matr(i_1, j_1) = data[i][j];
          j_1++;
        }
      }
      j_1 = 0;
      i_1++;
    }
  }
  return new_matr;
}

void Matrix::free_matrix() {
  for (size_t z = 0; z < rows; z++) {
    data[z].clear();
  }
  data.clear();
}

void Matrix::det_calc(const Matrix& matrix, double* val) const {
  double det_val = 0;
  if (matrix.getRows() != matrix.getCols()) throw(DimensionMismatch(*this));
  if (matrix.getRows() == 1) det_val = matrix(0, 0);
  if (matrix.getRows() == 2)
    det_val = matrix(0, 0) * matrix(1, 1) - matrix(0, 1) * matrix(1, 0);
  if (matrix.getRows() > 2) {
    short k = 1;
    for (size_t i = 0; i < matrix.getCols(); i++) {
      Matrix minor;
      minor = matrix.del_col_row(0, i);
      double val_minor = 1;
      det_calc(minor, &val_minor);
      det_val += k * matrix(0, i) * val_minor;
      k = -k;
      minor.free_matrix();
    }
  }
  *val = det_val;
}

double Matrix::det() const {
  double val = 0;
  Matrix::det_calc(*this, &val);

  return val;
}

Matrix Matrix::adj() const {
  if (rows != cols) throw(DimensionMismatch(*this));
  Matrix adj(rows, cols);

  if (rows == 1) {
    adj.data[0][0] = data[0][0];
    return adj;
  }

  int k;

  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      Matrix tmp_matrix = (*this).del_col_row(i, j);
      double val = tmp_matrix.det();
      k = pow(-1, i + 1 + j + 1);
      adj.data[i][j] = k * val;
      tmp_matrix.free_matrix();
    }
  }
  Matrix tmp = adj.transp();
  adj.free_matrix();
  return tmp;
}
Matrix Matrix::inv() const {
  if (rows != cols) throw(DimensionMismatch(*this));
  Matrix inv_m(rows, cols);

  if (rows == 1) {
    inv_m.data[0][0] = 1 / data[0][0];
    return inv_m;
  }
  double val = det();
  if (val == 0) throw SingularMatrix();
  Matrix adj_m = adj();
  Matrix tmp = adj_m * (1 / val);
  inv_m.free_matrix();
  adj_m.free_matrix();
  return tmp;
}

}  // namespace prep
