QT       += core gui opengl openglwidgets
include(view/QtGifImage-master/src/gifimage/qtgifimage.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += GL_SILENCE_DEPRECATION

VPATH += view/ libs/matrix/ controller/ model/

ICON = assets/icon.icns

SOURCES += \
    facade.cc \
    file_reader.cc \
    matrix.cc \
    preferences_model.cc \
    transform_matrix.cc \
    color_picker_button.cc \
    controller.cc \
    main.cc \
    preferences_widget.cc \
    viewer.cc \
    viewport.cc

HEADERS += \
    preferences.h \
    facade.h \
    figure.h \
    file_reader.h \
    matrix.h \
    preferences_model.h \
    transform_matrix.h \
    color_picker_button.h \
    controller.h \
    preferences_widget.h \
    viewer.h \
    viewport.h

FORMS += \
    preferences_widget.ui \
    viewer.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += view/ libs/matrix/ controller/ model/
