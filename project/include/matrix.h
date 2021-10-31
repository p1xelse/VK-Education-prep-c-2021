#ifndef PROJECT_INCLUDE_MATRIX_H_
#define PROJECT_INCLUDE_MATRIX_H_

#include <stddef.h>

#define COUNT_DIM 2


typedef struct Matrix {
  size_t n;
  size_t m;
  double **data;
} Matrix;

// Init/release operations
Matrix* create_matrix_from_file(const char* path_file);
Matrix* create_matrix(size_t rows, size_t cols);
void free_matrix(Matrix* matrix);

// Basic operations
int get_rows(const Matrix* matrix, size_t* rows);
int get_cols(const Matrix* matrix, size_t* cols);
int get_elem(const Matrix* matrix, size_t row, size_t col, double* val);
int set_elem(Matrix* matrix, size_t row, size_t col, double val);

// Math operations
Matrix* mul_scalar(const Matrix* matrix, double val);
Matrix* transp(const Matrix* matrix);

Matrix* sum(const Matrix* l, const Matrix* r);
Matrix* sub(const Matrix* l, const Matrix* r);
Matrix* mul(const Matrix* l, const Matrix* r);

// Extra operations
int det(const Matrix* matrix, double* val);
Matrix* adj(const Matrix* matrix);
Matrix* inv(const Matrix* matrix);



Matrix* create_matrix_without_col_row(const Matrix *matrix, size_t row, size_t col);
// void print_matrix(Matrix *m);
#endif  // PROJECT_INCLUDE_MATRIX_H_
