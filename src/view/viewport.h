#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>

#include "controller.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

namespace viewer {
class Viewport : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
 public:
  Viewport(QWidget *parent = nullptr, viewer::Controller *controller = nullptr);
  ~Viewport();
  auto OpenFile(const std::string &filepath) -> void;

 public slots:
  auto SetXRotation(int angle) -> void;
  auto SetYRotation(int angle) -> void;
  auto SetZRotation(int angle) -> void;
  auto SetXTranslation(double value) -> void;
  auto SetYTranslation(double value) -> void;
  auto SetZTranslation(double value) -> void;
  auto SetScale(double value) -> void;
  auto SetProjection(viewer::Preferences::ProjectionType projection_type)
      -> void;
  auto Cleanup() -> void;
  auto ApplyPreferences(viewer::Preferences preferences) -> void;

 signals:
  auto FileLoaded(const QString &filepath, unsigned int vertices_count,
                  unsigned int edges_count) -> void;
  auto XRotationChanged(int angle) -> void;
  auto YRotationChanged(int angle) -> void;
  auto ZRotationChanged(int angle) -> void;

 protected:
  auto initializeGL() -> void override;
  auto paintGL() -> void override;
  auto resizeGL(int width, int height) -> void override;
  auto mousePressEvent(QMouseEvent *event) -> void override;
  auto mouseMoveEvent(QMouseEvent *event) -> void override;

 private:
  auto NormalizeAngle(int &angle) -> void;
  auto CreateShaderProgram() -> void;
  auto CreateBuffers() -> void;
  auto DestroyBuffers() -> void;
  auto SetupVertexAttribs() -> void;
  auto SetupRotation() -> void;
  auto SetupTranslation() -> void;
  auto SetupScale() -> void;
  auto DrawLines() -> void;
  auto DrawPoints() -> void;
  auto SetBackgroundColor() -> void;
  auto SendObjectInfo(const QString &filepath) -> void;
  static constexpr GLuint kDashPattern = 0x0FF0;
  static constexpr GLfloat kProjectionZAxisTranslate = -5.0f;
  viewer::Controller *controller_;
  int x_rotation_ = 0;
  int y_rotation_ = 0;
  int z_rotation_ = 0;
  double x_translation_ = 0;
  double y_translation_ = 0;
  double z_translation_ = 0;
  double scale_ = 0;
  QPoint last_position_;
  viewer::Figure object_;
  QOpenGLVertexArrayObject vao_;
  QOpenGLBuffer vbo_;
  QOpenGLBuffer ebo_;
  QOpenGLShaderProgram *program_ = nullptr;
  int mvp_matrix_location_ = 0;
  int color_location_ = 0;
  QMatrix4x4 projection_matrix_;
  QMatrix4x4 object_matrix_;
  viewer::Preferences::ProjectionType projection_type_ =
      viewer::Preferences::kCentral;
  viewer::EdgePreferences::EdgeType line_type_ =
      viewer::EdgePreferences::kSolid;
  GLfloat line_width_;
  QColor line_color_;
  viewer::VertexPreferences::VertexType point_type_ =
      viewer::VertexPreferences::kNone;
  GLfloat point_size_;
  QColor point_color_;
  QColor background_color_;
  int viewport_width_;
  int viewport_height_;
};
}  // namespace viewer
#endif  // VIEWPORT_H
