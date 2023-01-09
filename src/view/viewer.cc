#include "viewer.h"

#include <QFileDialog>
#include <QGlobalStatic>

#include "ui_viewer.h"
#include "viewport.h"

static const QString pathToApplicationLocation = QDir::homePath();

Viewer::Viewer(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Viewer),
      controller_(new viewer::Controller),
      preferences_(new PreferencesWidget(nullptr, controller_)),
      viewport_(new viewer::Viewport(nullptr, controller_)),
      timer_(new QTimer(0)) {
  ui->setupUi(this);

  ui->view_section->insertWidget(1, viewport_);
  SetupRotationChangeConnections();
  SetupTranslationChangeConnections();
  SetupScaleChangeConnections();
  SetupActionMenusConnections();
  SetupLoadedFileConnections();
  connect(controller_->get_preferences_model(),
          &viewer::PreferencesModel::PreferencesChanged, viewport_,
          &viewer::Viewport::ApplyPreferences);
}

Viewer::~Viewer() {
  if (gif_) {
    delete gif_;
  }
  delete timer_;
  delete preferences_;
  delete controller_;
  delete viewport_;
  delete ui;
}

auto Viewer::OnOpenFileTriggered() -> void {
  QString filepath = QFileDialog::getOpenFileName(
      this, nullptr, pathToApplicationLocation, "OBJ (*.obj)", 0,
      QFileDialog::DontUseNativeDialog);
  viewport_->OpenFile(filepath.toStdString());
}

auto Viewer::OnSettingsTriggered() -> void { preferences_->show(); }

auto Viewer::OnScreenshotTriggered() -> void {
  QString save_path = QFileDialog::getSaveFileName(
      this, 0, pathToApplicationLocation, "JPEG (*.jpeg) ;; BMP (*.bmp)", 0,
      QFileDialog::DontUseNativeDialog);
  if (!save_path.isEmpty()) {
    size_t extension_pos = save_path.lastIndexOf('.') + 1;
    std::string extension =
        save_path.sliced(extension_pos).toUpper().toStdString();
    viewport_->grabFramebuffer().save(save_path, extension.c_str());
  }
}

auto Viewer::OnScreencastTriggered() -> void {
  if (gif_) {
    delete gif_;
  }
  gif_ = new QGifImage;
  gif_->setDefaultDelay(100);
  frame_counter_ = 0;
  connect(timer_, &QTimer::timeout, this, &Viewer::SaveFrame);
  timer_->start(100);
}

auto Viewer::OnFileLoaded(QString filename, unsigned int vertices_count,
                          unsigned int edges_count) -> void {
  ui->filename_display->setText(filename);
  ui->vertices_display->setText(QString::number(vertices_count));
  ui->edges_display->setText(QString::number(edges_count));
}

auto Viewer::SaveFrame() -> void {
  if (frame_counter_ < 50) {
    QImage frame = viewport_->grabFramebuffer().scaled(
        640, 480, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    gif_->addFrame(frame);
    ++frame_counter_;
  } else {
    timer_->stop();
    disconnect(timer_, &QTimer::timeout, this, &Viewer::SaveFrame);
    SaveGif();
  }
}

auto Viewer::SetupRotationChangeConnections() -> void {
  connect(ui->rotate_x_spinbox, &QSpinBox::valueChanged, viewport_,
          &viewer::Viewport::SetXRotation);
  connect(ui->rotate_y_spinbox, &QSpinBox::valueChanged, viewport_,
          &viewer::Viewport::SetYRotation);
  connect(ui->rotate_z_spinbox, &QSpinBox::valueChanged, viewport_,
          &viewer::Viewport::SetZRotation);
  connect(viewport_, &viewer::Viewport::XRotationChanged, ui->rotate_x_spinbox,
          &QSpinBox::setValue);
  connect(viewport_, &viewer::Viewport::YRotationChanged, ui->rotate_y_spinbox,
          &QSpinBox::setValue);
  connect(viewport_, &viewer::Viewport::ZRotationChanged, ui->rotate_z_spinbox,
          &QSpinBox::setValue);
}

auto Viewer::SetupTranslationChangeConnections() -> void {
  connect(ui->translate_x_spinbox, &QDoubleSpinBox::valueChanged, viewport_,
          &viewer::Viewport::SetXTranslation);
  connect(ui->translate_y_spinbox, &QDoubleSpinBox::valueChanged, viewport_,
          &viewer::Viewport::SetYTranslation);
  connect(ui->translate_z_spinbox, &QDoubleSpinBox::valueChanged, viewport_,
          &viewer::Viewport::SetZTranslation);
}

auto Viewer::SetupScaleChangeConnections() -> void {
  connect(ui->scale_spinbox, &QDoubleSpinBox::valueChanged, viewport_,
          &viewer::Viewport::SetScale);
}

void Viewer::SetupActionMenusConnections() {
  connect(ui->action_open_file, &QAction::triggered, this,
          &Viewer::OnOpenFileTriggered);
  connect(ui->action_preferences, &QAction::triggered, this,
          &Viewer::OnSettingsTriggered);
  connect(ui->action_screenshot, &QAction::triggered, this,
          &Viewer::OnScreenshotTriggered);
  connect(ui->action_screencast, &QAction::triggered, this,
          &Viewer::OnScreencastTriggered);
}

auto Viewer::SetupLoadedFileConnections() -> void {
  connect(viewport_, &viewer::Viewport::FileLoaded, this, &Viewer::OnFileLoaded);
}

auto Viewer::SaveGif() -> void {
  QString save_path = QFileDialog::getSaveFileName(
      this, NULL, pathToApplicationLocation, "GIF (*.gif)", 0,
      QFileDialog::DontUseNativeDialog);
  if (!save_path.isEmpty()) {
    gif_->save(save_path);
  }
}
