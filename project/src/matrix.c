#include "matrix.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_pointers_arr(double** data, int n) {
  for (int i = 0; i < n; i++) free(data[i]);

  free(data);
}

double** allocate_matrix_data(int n, int m) {
  if (m == 0 || n == 0) return NULL;
  double** data = calloc(n, sizeof(double*));

  if (!data) return NULL;

  for (int i = 0; i < n; i++) {
    data[i] = malloc(m * sizeof(double));

    if (!data[i]) {
      free_pointers_arr(data, n);
      return NULL;
    }
  }
  return data;
}

int read_dimensions(FILE* f, int* n, int* m) {
  if (fscanf(f, "%d %d", n, m) != COUNT_DIM) return EXIT_FAILURE;

  if (*n <= 0 || *m <= 0) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

Matrix* create_matrix_from_file(const char* path_file) {
  FILE* f = fopen(path_file, "r");

  if (f == NULL) return NULL;

  int n, m;
  int res_dim_r = read_dimensions(f, &n, &m);

  if (res_dim_r != EXIT_SUCCESS) {
    fclose(f);
    return NULL;
  }

  Matrix* matr = malloc(sizeof(Matrix));

  if (matr == NULL) {
    fclose(f);
    return NULL;
  }

  matr->n = n;
  matr->m = m;
  matr->data = allocate_matrix_data(n, m);

  if (matr->data == NULL) {
    free(matr);
    fclose(f);
    return NULL;
  }

  if (!matr->data) {
    free(matr);
    fclose(f);
    return NULL;
  }

  for (size_t i = 0; i < matr->n; i++)
    for (size_t j = 0; j < matr->m; j++)
      if (fscanf(f, "%lf", &matr->data[i][j]) != 1) {
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

  matr->n = rows;
  matr->m = cols;
  matr->data = allocate_matrix_data(rows, cols);

  if (!matr->data) {
    free(matr);
    return NULL;
  }

  return matr;
}

void free_matrix(Matrix* matrix) {
  free_pointers_arr(matrix->data, matrix->n);
  free(matrix);
}

int get_rows(const Matrix* matrix, size_t* rows) {
  if (matrix == NULL) return EXIT_FAILURE;

  *rows = matrix->n;

  return EXIT_SUCCESS;
}

int get_cols(const Matrix* matrix, size_t* cols) {
  if (matrix == NULL) return EXIT_FAILURE;

  *cols = matrix->m;

  return EXIT_SUCCESS;
}

int get_elem(const Matrix* matrix, size_t row, size_t col, double* val) {
  if (matrix == NULL) return EXIT_FAILURE;

  *val = matrix->data[row][col];

  return EXIT_SUCCESS;
}

int set_elem(Matrix* matrix, size_t row, size_t col, double val) {
  if (matrix == NULL) return EXIT_FAILURE;

  matrix->data[row][col] = val;

  return EXIT_SUCCESS;
}

Matrix* mul_scalar(const Matrix* matrix, double val) {
  if (matrix == NULL) return NULL;

  Matrix* new_matrix = create_matrix(matrix->n, matrix->m);

  for (size_t i = 0; i < new_matrix->n; i++)
    for (size_t j = 0; j < new_matrix->m; j++)
      new_matrix->data[i][j] = matrix->data[i][j] * val;

  return new_matrix;
}

Matrix* transp(const Matrix* matrix) {
  if (matrix == NULL) return NULL;

  Matrix* new_matrix = create_matrix(matrix->m, matrix->n);
  for (size_t i = 0; i < new_matrix->n; i++)
    for (size_t j = 0; j < new_matrix->m; j++)
      new_matrix->data[i][j] = matrix->data[j][i];

  return new_matrix;
}

Matrix* sum(const Matrix* l, const Matrix* r) {
  if (l == NULL || r == NULL) return NULL;
  if (l->m != r->m || l->n != r->n) return NULL;

  Matrix* new_matrix = create_matrix(l->n, l->m);
  for (size_t i = 0; i < new_matrix->n; i++)
    for (size_t j = 0; j < new_matrix->m; j++)
      new_matrix->data[i][j] = l->data[i][j] + r->data[i][j];

  return new_matrix;
}

Matrix* sub(const Matrix* l, const Matrix* r) {
  if (l == NULL || r == NULL) return NULL;
  if (l->m != r->m || l->n != r->n) return NULL;

  Matrix* new_matrix = create_matrix(l->n, l->m);
  for (size_t i = 0; i < new_matrix->n; i++)
    for (size_t j = 0; j < new_matrix->m; j++)
      new_matrix->data[i][j] = l->data[i][j] - r->data[i][j];

  return new_matrix;
}

Matrix* mul(const Matrix* l, const Matrix* r) {
  if (l == NULL || r == NULL) return NULL;
  if (l->m != r->n) return NULL;

  Matrix* new_matrix = create_matrix(l->n, r->m);

  for (size_t i = 0; i < l->n; i++) {
    for (size_t j = 0; j < r->m; j++) {
      new_matrix->data[i][j] = 0;
      for (size_t k = 0; k < l->m; k++)
        new_matrix->data[i][j] += l->data[i][k] * r->data[k][j];
    }
  }

  return new_matrix;
}

Matrix* create_matrix_without_col_row(const Matrix* matrix, size_t row,
                                      size_t col) {
  if (matrix == NULL) return NULL;
  Matrix* new_matrix = create_matrix(matrix->n - 1, matrix->m - 1);
  if (new_matrix == NULL) return NULL;
  size_t i_1 = 0;
  size_t j_1 = 0;

  for (size_t i = 0; i < matrix->n; i++) {
    if (i != row) {
      for (size_t j = 0; j < matrix->m; j++) {
        if (j != col) {
          new_matrix->data[i_1][j_1] = matrix->data[i][j];
          j_1++;
        }
      }
      j_1 = 0;
      i_1++;
    }
  }

  return new_matrix;
}

int det(const Matrix* matrix, double* val) {
  double det_val = 0;
  if (matrix->n != matrix->m) return EXIT_FAILURE;
  if (matrix->n == 1) det_val = matrix->data[0][0];

  if (matrix->n == 2)
    det_val = matrix->data[0][0] * matrix->data[1][1] -
              matrix->data[0][1] * matrix->data[1][0];

  if (matrix->n > 2) {
    short k = 1;
    for (size_t i = 0; i < matrix->m; i++) {
      Matrix* minor = create_matrix_without_col_row(matrix, 0, i);
      double val_minor = 1;
      det(minor, &val_minor);
      det_val += k * matrix->data[0][i] * val_minor;
      k = -k;
      free_matrix(minor);
    }
  }
  *val = det_val;

  return EXIT_SUCCESS;
}
Matrix* adj(const Matrix* matrix) {
  Matrix* adj = create_matrix(matrix->n, matrix->m);
  if (matrix->n == 1) {
    adj->data[0][0] = matrix->data[0][0];
    return adj;
  }

  int k;

  for (size_t i = 0; i < matrix->n; i++) {
    for (size_t j = 0; j < matrix->m; j++) {
      double val = 1;
      Matrix* tmp_matrix = create_matrix_without_col_row(matrix, i, j);
      det(tmp_matrix, &val);
      k = pow(-1, i + 1 + j + 1);
      adj->data[i][j] = k * val;
      free_matrix(tmp_matrix);
    }
  }
  Matrix* tmp = transp(adj);
  free_matrix(adj);
  return tmp;
}

Matrix* inv(const Matrix* matrix) {
  if (matrix == NULL) return NULL;

  Matrix* inv_m = create_matrix(matrix->n, matrix->m);

  if (inv_m == NULL) return NULL;

  if (matrix->n == 1) {
    inv_m->data[0][0] = 1 / matrix->data[0][0];
    return inv_m;
  }

  double val = 1;
  det(matrix, &val);
  Matrix* adj_m = adj(matrix);
  Matrix* tmp = mul_scalar((const Matrix*)adj_m, 1 / val);
  free_matrix(inv_m);
  free_matrix(adj_m);
  return tmp;
}
