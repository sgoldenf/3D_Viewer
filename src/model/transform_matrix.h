#ifndef TRANSFORM_MATRIX_H
#define TRANSFORM_MATRIX_H

#include <matrix.h>

#include <cmath>

#include "figure.h"

namespace viewer {
class TransformMatrix {
 public:
  explicit TransformMatrix(matrix::Matrix matrix) : matrix_(matrix){};
  ~TransformMatrix() {}

  auto get_matrix() const -> matrix::Matrix { return matrix_; }
  auto TransformPoint(Point point) -> Point;

 private:
  matrix::Matrix matrix_;
};

class TransformMatrixBuilder {
 public:
  TransformMatrixBuilder();
  TransformMatrixBuilder(const TransformMatrixBuilder&) = delete;
  TransformMatrixBuilder& operator=(TransformMatrixBuilder&&) = delete;
  virtual ~TransformMatrixBuilder();
  virtual auto BuildTransformMatrix(double x, double y, double z) -> void = 0;
  auto get_transform_matrix() -> TransformMatrix*;

 protected:
  TransformMatrix* transform_matrix_ = nullptr;
};

class RotationMatrixBuilder : public TransformMatrixBuilder {
 public:
  auto BuildTransformMatrix(double x, double y, double z) -> void override;
};

class TranslationMatrixBuilder : public TransformMatrixBuilder {
 public:
  auto BuildTransformMatrix(double x, double y, double z) -> void override;
};

class ScaleMatrixBuilder : public TransformMatrixBuilder {
 public:
  auto BuildTransformMatrix(double x, double y, double z) -> void override;
};
}  // namespace viewer

#endif  // TRANSFORM_MATRIX_H
