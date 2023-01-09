#include "transform_matrix.h"

namespace viewer {
TransformMatrixBuilder::TransformMatrixBuilder() {
  matrix::Matrix matrix(4, 4);
  matrix.SetToIdentity();
  transform_matrix_ = new TransformMatrix(matrix);
}

TransformMatrixBuilder::~TransformMatrixBuilder() { delete transform_matrix_; }

auto TransformMatrix::TransformPoint(Point point) -> Point {
  Point result_point;
  matrix::Matrix point_matrix(4, 1);
  point_matrix(0, 0) = point.x;
  point_matrix(1, 0) = point.y;
  point_matrix(2, 0) = point.z;
  point_matrix(3, 0) = 1;
  matrix::Matrix result_matrix = matrix_ * point_matrix;
  result_point.x = result_matrix(0, 0);
  result_point.y = result_matrix(1, 0);
  result_point.z = result_matrix(2, 0);
  return result_point;
}

auto TransformMatrixBuilder::get_transform_matrix() -> TransformMatrix* {
  return transform_matrix_;
}

auto RotationMatrixBuilder::BuildTransformMatrix(double x, double y, double z)
    -> void {
  matrix::Matrix matrix(4, 4);
  matrix(0, 0) = cos(y) * cos(z);
  matrix(0, 1) = -sin(z) * cos(x) + cos(z) * sin(y) * sin(x);
  matrix(0, 2) = sin(z) * sin(x) + cos(z) * sin(y) * cos(x);
  matrix(1, 0) = sin(z) * cos(y);
  matrix(1, 1) = cos(z) * cos(x) + sin(z) * sin(y) * sin(x);
  matrix(1, 2) = -cos(z) * sin(x) + sin(z) * sin(y) * cos(x);
  matrix(2, 0) = -sin(y);
  matrix(2, 1) = cos(y) * sin(x);
  matrix(2, 2) = cos(x) * cos(y);
  matrix(3, 3) = 1;
  if (transform_matrix_) {
    delete transform_matrix_;
  }
  transform_matrix_ = new TransformMatrix(matrix);
}

auto TranslationMatrixBuilder::BuildTransformMatrix(double x, double y,
                                                    double z) -> void {
  matrix::Matrix matrix(4, 4);
  matrix.SetToIdentity();
  const double translate_values[3] = {x, y, z};
  for (int i = 0; i < 3; ++i) {
    matrix(i, 3) = translate_values[i];
  }
  if (transform_matrix_) {
    delete transform_matrix_;
  }
  transform_matrix_ = new TransformMatrix(matrix);
}

auto ScaleMatrixBuilder::BuildTransformMatrix(double x, double y, double z)
    -> void {
  matrix::Matrix matrix(4, 4);
  const double scale_values[4] = {x, y, z, 1};
  for (int i = 0; i < 4; ++i) {
    matrix(i, i) = scale_values[i];
  }
  if (transform_matrix_) {
    delete transform_matrix_;
  }
  transform_matrix_ = new TransformMatrix(matrix);
}
}  // namespace viewer
