#ifndef PREFERENCESWIDGET_H
#define PREFERENCESWIDGET_H

#include <QWidget>

#include "color_picker_button.h"
#include "controller.h"

namespace Ui {
class PreferencesWidget;
}

class PreferencesWidget : public QWidget {
  Q_OBJECT

 public:
  explicit PreferencesWidget(QWidget* parent = nullptr,
                             viewer::Controller* controller = nullptr);
  ~PreferencesWidget();

 public slots:
  auto OnApplyButtonClicked() -> void;
  auto OnCloseButtonClicked() -> void;
  auto OnSettingChanged() -> void;

 private:
  auto ConnectToSaveSettings() -> void;
  auto RestoreSettingsFromModel() -> void;
  auto BuildPreferencesFromWidgets() -> viewer::Preferences;
  auto BuildEdgePreferencesFromWidgets() -> viewer::EdgePreferences;
  auto BuildVertexPreferencesFromWidgets() -> viewer::VertexPreferences;

  Ui::PreferencesWidget* ui;
  viewer::Controller* controller_;
  viewer::PreferencesMemento* memento_;
  viewer::ColorPickerButton* edge_color_button_;
  viewer::ColorPickerButton* vertex_color_button_;
  viewer::ColorPickerButton* background_color_button_;
};

#endif  // PREFERENCESWIDGET_H
