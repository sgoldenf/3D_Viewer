#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QTimer>

#include "controller.h"
#include "preferences_widget.h"
#include "qgifimage.h"
#include "viewport.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Viewer;
}
QT_END_NAMESPACE

class Viewer : public QMainWindow {
  Q_OBJECT

 public:
  Viewer(QWidget *parent = nullptr);
  ~Viewer();

 public slots:
  auto OnOpenFileTriggered() -> void;
  auto OnSettingsTriggered() -> void;
  auto OnScreenshotTriggered() -> void;
  auto OnScreencastTriggered() -> void;
  auto OnFileLoaded(QString filename, unsigned int vertices_count,
                    unsigned int edges_count) -> void;
  auto SaveFrame() -> void;

 private:
  auto SetupRotationChangeConnections() -> void;
  auto SetupTranslationChangeConnections() -> void;
  auto SetupScaleChangeConnections() -> void;
  auto SetupProjectionTypeChangeConnections() -> void;
  auto SetupActionMenusConnections() -> void;
  auto SetupLoadedFileConnections() -> void;
  auto SaveGif() -> void;

  Ui::Viewer *ui;
  viewer::Controller *controller_;
  PreferencesWidget *preferences_;
  viewer::Viewport *viewport_;
  QGifImage *gif_ = nullptr;
  QTimer *timer_ = nullptr;
  unsigned int frame_counter_ = 0;
};
#endif  // VIEWER_H
