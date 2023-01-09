#include "viewport.h"

#include <QColor>
#include <QMouseEvent>
#include <QOpenGLShaderProgram>

namespace viewer {
static const QString vertex_shader_code =
    "#ifdef GL_ES\n"
    "precision mediump int;\n"
    "precision mediump float;\n"
    "#endif\n"
    "attribute vec4 vertex;\n"
    "uniform mat4 mvp_matrix;\n"
    "\n"
    "void main()\n"
    "{\n"
    "gl_Position = mvp_matrix * vertex;\n"
    "}\n";

static const QString fragment_shader_code =
    "#ifdef GL_ES\n"
    "precision mediump int;\n"
    "precision mediump float;\n"
    "#endif\n"
    "uniform vec4 color;\n"
    "void main()\n"
    "{\n"
    "gl_FragColor = color;\n"
    "}\n";

Viewport::Viewport(QWidget *parent, viewer::Controller *controller)
    : QOpenGLWidget(parent), controller_(controller) {
  ApplyPreferences(controller_->get_preferences());
}

Viewport::~Viewport() { Cleanup(); }

void Viewport::OpenFile(const std::string &filepath) {
  if (!filepath.empty()) {
    object_ = controller_->ParseFigure(filepath);
    DestroyBuffers();
    CreateBuffers();
    SendObjectInfo(QString::fromStdString(filepath));
    update();
  }
}

void Viewport::SetXRotation(int angle) {
  NormalizeAngle(angle);
  if (angle != x_rotation_) {
    x_rotation_ = angle;
    SetupRotation();
    emit XRotationChanged(angle);
    update();
  }
}

void Viewport::SetYRotation(int angle) {
  NormalizeAngle(angle);
  if (angle != y_rotation_) {
    y_rotation_ = angle;
    SetupRotation();
    emit YRotationChanged(angle);
    update();
  }
}

void Viewport::SetZRotation(int angle) {
  NormalizeAngle(angle);
  if (angle != z_rotation_) {
    z_rotation_ = angle;
    SetupRotation();
    emit ZRotationChanged(angle);
    update();
  }
}

void Viewport::SetXTranslation(double value) {
  x_translation_ = value;
  SetupTranslation();
  update();
}

void Viewport::SetYTranslation(double value) {
  y_translation_ = value;
  SetupTranslation();
  update();
}

void Viewport::SetZTranslation(double value) {
  z_translation_ = value;
  SetupTranslation();
  update();
}

void Viewport::SetScale(double value) {
  scale_ = value;
  SetupScale();
  update();
}

void Viewport::SetProjection(
    viewer::Preferences::ProjectionType projection_type) {
  GLfloat aspect_ratio = GLfloat(viewport_width_) / viewport_height_;
  projection_matrix_.setToIdentity();
  if (projection_type == viewer::Preferences::kCentral) {
    GLfloat fov = 45.0f;
    GLfloat near = 0.01f;
    GLfloat far = 1000.0f;
    projection_matrix_.perspective(fov, aspect_ratio, near, far);
    projection_matrix_.translate(0, 0, kProjectionZAxisTranslate);
  } else {
    GLfloat left = -2.0f;
    GLfloat right = -left;
    GLfloat bottom = left / aspect_ratio;
    GLfloat top = -bottom;
    GLfloat near = -100.0f;
    GLfloat far = -near;
    projection_matrix_.ortho(left, right, bottom, top, near, far);
  }
  update();
}

void Viewport::Cleanup() {
  if (program_ == nullptr) {
    return;
  }
  makeCurrent();
  DestroyBuffers();
  delete program_;
  program_ = nullptr;
  doneCurrent();
}

void Viewport::ApplyPreferences(viewer::Preferences preferences) {
  projection_type_ = preferences.projection_type_;
  line_type_ = preferences.edge_.type_;
  line_width_ = preferences.edge_.thickness_;
  line_color_ = preferences.edge_.color_;
  point_type_ = preferences.vertex_.type_;
  point_size_ = preferences.vertex_.size_;
  point_color_ = preferences.vertex_.color_;
  background_color_ = preferences.background_color_;
  update();
}

void Viewport::initializeGL() {
  initializeOpenGLFunctions();
  CreateShaderProgram();
  program_->bind();
}

void Viewport::paintGL() {
  SetBackgroundColor();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  SetProjection(projection_type_);
  object_matrix_ = controller_->GetTransformMatrix();
  program_->setUniformValue(mvp_matrix_location_,
                            projection_matrix_ * object_matrix_);
  QOpenGLVertexArrayObject::Binder vaoBinder(&vao_);
  vao_.bind();
  DrawLines();
  DrawPoints();
  vao_.release();
}

void Viewport::resizeGL(int w, int h) {
  viewport_width_ = w;
  viewport_height_ = h;
}

void Viewport::mousePressEvent(QMouseEvent *event) {
  last_position_ = event->position().toPoint();
}

void Viewport::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->position().toPoint().x() - last_position_.x();
  int dy = event->position().toPoint().y() - last_position_.y();

  if (event->buttons() & Qt::LeftButton) {
    SetXRotation(x_rotation_ + dy);
    SetYRotation(y_rotation_ + dx);
  } else if (event->buttons() & Qt::RightButton) {
    SetXRotation(x_rotation_ + dy);
    SetZRotation(z_rotation_ + dx);
  }
  last_position_ = event->position().toPoint();
}

void Viewport::NormalizeAngle(int &angle) {
  while (angle < -360) {
    angle += 720;
  }
  while (angle > 360) {
    angle -= 720;
  }
}

void Viewport::CreateShaderProgram() {
  program_ = new QOpenGLShaderProgram;
  program_->addShaderFromSourceCode(QOpenGLShader::Vertex, vertex_shader_code);
  program_->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                    fragment_shader_code);
  program_->bindAttributeLocation("vertex", 0);
  program_->link();
  mvp_matrix_location_ = program_->uniformLocation("mvp_matrix");
  color_location_ = program_->uniformLocation("color");
}

void Viewport::CreateBuffers() {
  vao_.create();
  QOpenGLVertexArrayObject::Binder vaoBinder(&vao_);

  vbo_.create();
  vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vbo_.bind();
  vbo_.allocate(object_.get_data(),
                object_.get_vertices().size() * sizeof(viewer::Point));

  ebo_ = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
  ebo_.create();
  ebo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  ebo_.bind();
  ebo_.allocate(object_.get_indices().data(),
                object_.get_indices().size() * sizeof(GLuint));
  SetupVertexAttribs();
}

void Viewport::DestroyBuffers() {
  vbo_.destroy();
  ebo_.destroy();
  vao_.destroy();
}

void Viewport::SetupVertexAttribs() {
  vbo_.bind();
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  f->glEnableVertexAttribArray(0);
  f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                           nullptr);
  vbo_.release();
}

void Viewport::SetupRotation() {
  double x = (x_rotation_ * M_PI) / 180.0;
  double y = (y_rotation_ * M_PI) / 180.0;
  double z = (z_rotation_ * M_PI) / 180.0;
  controller_->Rotate(x, y, z);
}

void Viewport::SetupTranslation() {
  controller_->Translate(x_translation_, y_translation_, z_translation_);
}

void Viewport::SetupScale() { controller_->Scale(scale_, scale_, scale_); }

void Viewport::DrawLines() {
  glLineWidth(line_width_);
  if (line_type_ == viewer::EdgePreferences::kDashed) {
    glLineStipple(1, kDashPattern);
    glEnable(GL_LINE_STIPPLE);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }
  program_->setUniformValue(color_location_, line_color_);
  glDrawElements(GL_LINES, object_.get_indices().size(), GL_UNSIGNED_INT,
                 nullptr);
}

void Viewport::DrawPoints() {
  if (point_type_ == viewer::VertexPreferences::kNone) {
    return;
  }
  glPointSize(point_size_);
  if (point_type_ == viewer::VertexPreferences::kCircle) {
    glEnable(GL_POINT_SMOOTH);
  } else {
    glDisable(GL_POINT_SMOOTH);
  }
  program_->setUniformValue(color_location_, point_color_);
  glDrawArrays(GL_POINTS, 0, object_.get_vertices().size());
}

void Viewport::SetBackgroundColor() {
  float r, g, b, a;
  background_color_.getRgbF(&r, &g, &b, &a);
  glClearColor(r, g, b, a);
}

void Viewport::SendObjectInfo(const QString &filepath) {
  size_t pos = filepath.lastIndexOf('/') + 1;
  QString filename = filepath.sliced(pos);
  unsigned int vertices_count = object_.get_vertices().size();
  unsigned int edges_count =
      object_.get_vertices().size() + object_.get_facets_count() - 2;
  emit FileLoaded(filename, vertices_count, edges_count);
}
}  // namespace viewer
