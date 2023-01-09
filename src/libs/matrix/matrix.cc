#include "matrix.h"

#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace matrix {
Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  if (rows <= 0 || cols <= 0)
    throw std::invalid_argument("Invalid arguments for rows and columns");
  matrix_ = AllocateMemory();
}

Matrix::Matrix(const Matrix& other) : rows_(other.rows_), cols_(other.cols_) {
  matrix_ = AllocateMemory();
  CopyMatrix(matrix_, other.matrix_, rows_, cols_);
}

Matrix::Matrix(Matrix&& other) : rows_(0), cols_(0), matrix_(nullptr) {
  std::swap(rows_, other.rows_);
  std::swap(cols_, other.cols_);
  std::swap(matrix_, other.matrix_);
}

Matrix::~Matrix() { FreeMatrix(); }

void Matrix::set_rows(int rows) {
  if (rows <= 0) throw std::invalid_argument("Invalid argument for rows");
  if (rows != rows_) {
    int maxRow = rows < rows_ ? rows : rows_;
    rows_ = rows;
    double** newMatrix = AllocateMemory();
    CopyMatrix(newMatrix, matrix_, maxRow, cols_);
    FreeMatrix();
    matrix_ = newMatrix;
  }
}

void Matrix::set_cols(int cols) {
  if (cols <= 0) throw std::invalid_argument("Invalid argument for cols");
  if (cols != cols_) {
    int maxCol = cols < cols_ ? cols : cols_;
    cols_ = cols;
    double** newMatrix = AllocateMemory();
    CopyMatrix(newMatrix, matrix_, rows_, maxCol);
    FreeMatrix();
    matrix_ = newMatrix;
  }
}

Matrix operator+(const Matrix& A, const Matrix& B) {
  if (A.rows_ != B.rows_ || A.cols_ != B.cols_)
    throw std::invalid_argument("Different matrix dimensions");
  Matrix sum(A);
  sum.SumMatrix(B);
  return sum;
}

Matrix operator-(const Matrix& A, const Matrix& B) {
  if (A.rows_ != B.rows_ || A.cols_ != B.cols_)
    throw std::invalid_argument("Different matrix dimensions");
  Matrix sub(A);
  sub.SubMatrix(B);
  return sub;
}

Matrix operator*(const Matrix& A, const Matrix& B) {
  if (A.cols_ != B.rows_)
    throw std::invalid_argument("Different matrix dimensions");
  Matrix mul(A);
  mul.Multiply(B);
  return mul;
}

Matrix operator*(const Matrix& matrix, double number) {
  Matrix mul(matrix);
  mul.Multiply(number);
  return mul;
}

Matrix operator*(double number, const Matrix& matrix) {
  Matrix mul(matrix);
  mul.Multiply(number);
  return mul;
}

bool operator==(const Matrix& A, const Matrix& B) {
  if (&A == &B) return true;
  return A.IsEqualToMatrix(B);
}

Matrix& Matrix::operator=(const Matrix& other) {
  if (this == &other) return *this;
  rows_ = other.rows_;
  cols_ = other.cols_;
  FreeMatrix();
  matrix_ = AllocateMemory();
  CopyMatrix(matrix_, other.matrix_, rows_, cols_);
  return *this;
}

double& Matrix::operator()(int row, int col) {
  if (row < 0 || row >= rows_ || col < 0 || col >= cols_)
    throw std::invalid_argument("Invalid arguments for row and col");
  return matrix_[row][col];
}

bool Matrix::IsEqualToMatrix(const Matrix& other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_) return false;
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      if (fabs(matrix_[i][j] - other.matrix_[i][j]) > kDiff) return false;
    }
  }
  return true;
}

void Matrix::SumMatrix(const Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw std::invalid_argument("Different matrix dimensions");
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] += other.matrix_[i][j];
    }
  }
}

void Matrix::SubMatrix(const Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw std::invalid_argument("Different matrix dimensions");
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

void Matrix::Multiply(const double num) {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) matrix_[i][j] *= num;
  }
}

void Matrix::Multiply(const Matrix& other) {
  if (cols_ != other.rows_)
    throw std::invalid_argument("Different matrix dimensions");
  Matrix result(rows_, other.cols_);
  for (int i = 0; i < result.rows_; ++i) {
    for (int j = 0; j < result.cols_; ++j) {
      result.matrix_[i][j] = 0.0;
      for (int k = 0; k < cols_; ++k)
        result.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
    }
  }
  *this = result;
}

Matrix Matrix::Transpose() {
  Matrix transposed(cols_, rows_);
  for (int i = 0; i < transposed.rows_; ++i) {
    for (int j = 0; j < transposed.cols_; ++j) {
      transposed.matrix_[i][j] = matrix_[j][i];
    }
  }
  return transposed;
}

Matrix Matrix::CalcComplements() {
  if (rows_ != cols_) throw std::invalid_argument("Matrix isn't square");
  Matrix complements(rows_, cols_);
  if (rows_ == 1) {
    complements.matrix_[0][0] = 1;
  } else {
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        Matrix amatrix = AdditionalMatrix(i, j);
        complements.matrix_[i][j] = pow(-1, i + j) * amatrix.Determinant();
      }
    }
  }
  return complements;
}

double Matrix::Determinant() {
  if (rows_ != cols_) throw std::invalid_argument("Matrix isn't square");
  double result = 0.0;
  if (rows_ == 1) {
    result = matrix_[0][0];
  } else if (rows_ == 2) {
    result = matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
  } else {
    for (int i = 0; i < rows_; ++i) {
      Matrix amatrix = AdditionalMatrix(i, 0);
      result += pow(-1, i) * matrix_[i][0] * amatrix.Determinant();
    }
  }
  return result;
}

Matrix Matrix::Inverse() {
  double det = Determinant();
  if (fabs(det) <= kDiff)
    throw std::invalid_argument("Determinant equals zero");
  Matrix inversed = CalcComplements().Transpose() * (1.0 / det);
  return inversed;
}

void Matrix::SetToIdentity() {
  if (get_rows() != get_cols()) {
    return;
  }
  for (int i = 0; i < get_rows(); ++i) {
    (*this)(i, i) = 1;
  }
}

Matrix Matrix::AdditionalMatrix(int skip_row, int skip_column) {
  Matrix additional(rows_ - 1, cols_ - 1);
  for (int i = 0, res_i = 0; i < rows_; ++i) {
    if (i != skip_row) {
      for (int j = 0, res_j = 0; j < cols_; ++j) {
        if (j != skip_column) {
          additional.matrix_[res_i][res_j] = matrix_[i][j];
          ++res_j;
        }
      }
      ++res_i;
    }
  }
  return additional;
}

double** Matrix::AllocateMemory() {
  double** matrix = new double*[rows_];
  matrix[0] = new double[rows_ * cols_]{};
  for (int i = 1; i < rows_; ++i) matrix[i] = matrix[i - 1] + cols_;
  return matrix;
}

void Matrix::CopyMatrix(double** copy, double** matrix, int rows, int cols) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) copy[i][j] = matrix[i][j];
  }
}

void Matrix::FreeMatrix() {
  if (matrix_ != nullptr) {
    delete[] matrix_[0];
    delete[] matrix_;
    matrix_ = nullptr;
  }
}
}  // namespace matrix
