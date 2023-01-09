#include <QApplication>
#include <QScreen>
#include <QSurfaceFormat>

#include "viewer.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  Viewer viewer;
  viewer.show();
  return app.exec();
}
