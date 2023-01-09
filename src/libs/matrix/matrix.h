#ifndef MATRIX_H_
#define MATRIX_H_

namespace matrix {
class Matrix {
 public:
  Matrix() : Matrix(3, 3) {}
  Matrix(int rows, int cols);
  Matrix(const Matrix& other);
  Matrix(Matrix&& other);
  ~Matrix();

  int get_rows() { return rows_; }
  int get_cols() { return cols_; }
  void set_rows(int rows);
  void set_cols(int cols);

  friend Matrix operator+(const Matrix& A, const Matrix& B);
  friend Matrix operator-(const Matrix& A, const Matrix& B);
  friend Matrix operator*(const Matrix& A, const Matrix& B);
  friend Matrix operator*(const Matrix& matrix, double number);
  friend Matrix operator*(double number, const Matrix& matrix);
  friend bool operator==(const Matrix& A, const Matrix& B);
  Matrix& operator=(const Matrix& other);
  void operator+=(const Matrix& other) { SumMatrix(other); }
  void operator-=(const Matrix& other) { SubMatrix(other); }
  void operator*=(double number) { Multiply(number); }
  void operator*=(const Matrix& other) { Multiply(other); }
  double& operator()(int row, int col);

  bool IsEqualToMatrix(const Matrix& other) const;
  void SumMatrix(const Matrix& other);
  void SubMatrix(const Matrix& other);
  void Multiply(const double num);
  void Multiply(const Matrix& other);
  Matrix Transpose();
  Matrix CalcComplements();
  double Determinant();
  Matrix Inverse();
  void SetToIdentity();

 private:
  constexpr static double kDiff = 1e-7;
  int rows_, cols_;
  double** matrix_;

  double** AllocateMemory();
  void CopyMatrix(double** copy, double** matrix, int rows, int cols);
  void FreeMatrix();
  Matrix AdditionalMatrix(int skip_row, int skip_column);
};
}  // namespace matrix

#endif  // MATRIX_H_
