#include "matrix.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void free_matrix_data(Matrix *matrix) {
  free(matrix->data);
}

double* allocate_matrix_data(int rows, int cols) {
  double* data;
  if (0 == rows || 0 == cols) return NULL;
  data = malloc(sizeof(double) * rows * cols);
  return data;
}

int read_dimensions(FILE* f, int* n, int* m) {
  if (fscanf(f, "%d %d", n, m) != 2) return EXIT_FAILURE;

  if (*n <= 0 || *m <= 0)
    return EXIT_FAILURE;

  return 0;
}

Matrix* create_matrix_from_file(const char* path_file) {
  FILE* f = fopen(path_file, "r");

  if (f == NULL) return NULL;

  int rows, cols;
  int res_dim_r = read_dimensions(f, &rows, &cols);

  if (res_dim_r != 0) {
    fclose(f);
    return NULL;
  }

  Matrix* matr = create_matrix(rows, cols);

  if (matr == NULL) {
    fclose(f);
    return NULL;
  }

  for (size_t i = 0; i < matr->rows; i++)
    for (size_t j = 0; j < matr->cols; j++)
      if (fscanf(f, "%lf", &matr->data[i * matr->cols + j]) != 1) {
        free_matrix(matr);
        fclose(f);
        return NULL;
      }

  fclose(f);
  return matr;
}

Matrix* create_matrix(size_t rows, size_t cols) {
  Matrix* matr = malloc(sizeof(Matrix));

  if (matr == NULL) return NULL;

  matr->rows = rows;
  matr->cols = cols;
  matr->data = allocate_matrix_data(rows, cols);

  if (!matr->data) {
    free(matr);
    return NULL;
  }

  return matr;
}

void free_matrix(Matrix* matrix) {
  free_matrix_data(matrix);
  free(matrix);
}

int get_rows(const Matrix* matrix, size_t* rows) {
  if (matrix == NULL) return EXIT_FAILURE;

  *rows = matrix->rows;

  return 0;
}

int get_cols(const Matrix* matrix, size_t* cols) {
  if (matrix == NULL) return EXIT_FAILURE;

  *cols = matrix->cols;

  return 0;
}

int get_elem(const Matrix* matrix, size_t row, size_t col, double* val) {
  if (matrix == NULL)
    return EXIT_FAILURE;
  if (row > matrix->rows - 1 || col > matrix->cols - 1)
    return EXIT_FAILURE;

  *val = matrix->data[matrix->cols * row + col];

  return 0;
}

int set_elem(Matrix* matrix, size_t row, size_t col, double val) {
  if (matrix == NULL)
    return EXIT_FAILURE;

  if (row > matrix->rows - 1 || col > matrix->cols - 1)
    return EXIT_FAILURE;
  matrix->data[matrix->cols * row + col] = val;

  return 0;
}

Matrix* mul_scalar(const Matrix* matrix, double val) {
  if (matrix == NULL) return NULL;

  Matrix* new_matrix = create_matrix(matrix->rows, matrix->cols);

  if (new_matrix == NULL)
    return NULL;

  for (size_t i = 0; i < new_matrix->rows; i++)
    for (size_t j = 0; j < new_matrix->cols; j++)
      new_matrix->data[i * matrix->cols + j] =
          matrix->data[i * matrix->cols + j] * val;

  return new_matrix;
}

Matrix* transp(const Matrix* matrix) {
  if (matrix == NULL) return NULL;

  Matrix* new_matrix = create_matrix(matrix->cols, matrix->rows);
  for (size_t i = 0; i < new_matrix->rows; i++)
    for (size_t j = 0; j < new_matrix->cols; j++)
      new_matrix->data[i * new_matrix->cols + j] = matrix->data[j * matrix->cols + i];

  return new_matrix;
}

Matrix* sum(const Matrix* l, const Matrix* r) {
  if (l == NULL || r == NULL) return NULL;
  if (l->cols != r->cols || l->rows != r->rows) return NULL;

  Matrix* new_matrix = create_matrix(l->rows, l->cols);
  for (size_t i = 0; i < new_matrix->rows; i++)
    for (size_t j = 0; j < new_matrix->cols; j++)
      new_matrix->data[i * new_matrix->cols + j] =
          l->data[i * l->cols + j] + r->data[i * r->cols + j];

  return new_matrix;
}

Matrix* sub(const Matrix* l, const Matrix* r) {
  if (l == NULL || r == NULL) return NULL;
  if (l->cols != r->cols || l->rows != r->rows) return NULL;

  Matrix* new_matrix = create_matrix(l->rows, l->cols);
  for (size_t i = 0; i < new_matrix->rows; i++)
    for (size_t j = 0; j < new_matrix->cols; j++)
      new_matrix->data[i * new_matrix->cols + j] =
          l->data[i * l->cols + j] - r->data[i * r->cols + j];

  return new_matrix;
}

Matrix* mul(const Matrix* l, const Matrix* r) {
  if (l == NULL || r == NULL) return NULL;
  if (l->cols != r->rows) return NULL;

  Matrix* new_matrix = create_matrix(l->rows, r->cols);

  for (size_t i = 0; i < l->rows; i++) {
    for (size_t j = 0; j < r->cols; j++) {
      new_matrix->data[i * new_matrix->cols + j] = 0;
      for (size_t k = 0; k < l->cols; k++)
        new_matrix->data[i * new_matrix->cols + j] +=
            l->data[i * l->cols + k] * r->data[k * r->cols + j];
    }
  }

  return new_matrix;
}

static Matrix* create_matrix_without_col_row(const Matrix* matrix, size_t row,
                                      size_t col) {
  if (matrix == NULL) return NULL;
  Matrix* new_matrix = create_matrix(matrix->rows - 1, matrix->cols - 1);
  if (new_matrix == NULL) return NULL;
  size_t new_row_index = 0;
  size_t new_col_index = 0;

  for (size_t i = 0; i < matrix->rows; i++) {
    if (i != row) {
      for (size_t j = 0; j < matrix->cols; j++) {
        if (j != col) {
          new_matrix->data[new_row_index * new_matrix->cols + new_col_index] =
              matrix->data[i * matrix->cols + j];
          new_col_index++;
        }
      }
      new_col_index = 0;
      new_row_index++;
    }
  }

  return new_matrix;
}

int det(const Matrix* matrix, double* val) {
  double det_val = 0;
  if (matrix->rows != matrix->cols) {
    return EXIT_FAILURE;
  } else if (matrix->rows == 1) {
    det_val = matrix->data[0];
  } else if (matrix->rows == 2) {
    det_val = matrix->data[0] * matrix->data[matrix->cols + 1] -
              matrix->data[1] * matrix->data[matrix->cols];
  } else if (matrix->rows > 2) {
    char k = 1;
    for (size_t i = 0; i < matrix->cols; i++) {
      Matrix* minor = create_matrix_without_col_row(matrix, 0, i);

      if (minor == NULL)
        return EXIT_FAILURE;

      double val_minor = 1;
      det(minor, &val_minor);
      det_val += k * matrix->data[i] * val_minor;
      k = -k;
      free_matrix(minor);
    }
  }
  *val = det_val;

  return 0;
}
Matrix* adj(const Matrix* matrix) {
  Matrix* adj = create_matrix(matrix->rows, matrix->cols);
  if (matrix->rows == 1) {
    adj->data[0] = matrix->data[0];
    return adj;
  }


  for (size_t i = 0; i < matrix->rows; i++) {
    for (size_t j = 0; j < matrix->cols; j++) {
      double val = 1;
      Matrix* tmp_matrix = create_matrix_without_col_row(matrix, i, j);
      det(tmp_matrix, &val);

      int k;
      k = ((i + 1 + j + 1) % 2 == 0) ? 1 : -1;

      adj->data[i * adj->cols + j] = k * val;

      free_matrix(tmp_matrix);
    }
  }
  Matrix* tmp = transp(adj);
  free_matrix(adj);
  return tmp;
}

Matrix* inv(const Matrix* matrix) {
  if (matrix == NULL) return NULL;

  Matrix* inv_matrix = create_matrix(matrix->rows, matrix->cols);

  if (inv_matrix == NULL) return NULL;

  if (matrix->rows == 1) {
    inv_matrix->data[0] = 1 / matrix->data[0];
    return inv_matrix;
  }

  double val = 1;
  det(matrix, &val);
  Matrix* adj_m = adj(matrix);
  Matrix* tmp = mul_scalar(adj_m, 1 / val);
  free_matrix(inv_matrix);
  free_matrix(adj_m);

  return tmp;
}
