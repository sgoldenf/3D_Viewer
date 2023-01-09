#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "facade.h"

namespace viewer {
class Controller {
 public:
  Controller();
  Controller(const Controller&) = delete;
  Controller& operator=(Controller&&) = delete;
  ~Controller();

  auto ParseFigure(const std::string& path) -> const Figure&;
  auto Rotate(double x, double y, double z) -> void;
  auto Translate(double x, double y, double z) -> void;
  auto Scale(double x, double y, double z) -> void;
  auto GetTransformMatrix() -> QMatrix4x4;
  auto SavePreferences() -> PreferencesMemento*;
  auto SavePreferencesToFile() -> void { model_->SavePreferencesToFile(); };
  auto ReinstatePreferences(PreferencesMemento* memento) -> void;

  auto get_preferences_model() -> PreferencesModel* {
    return model_->get_preferences_model();
  };
  auto get_preferences() -> Preferences;
  auto set_preferences(const Preferences& preferences) -> void;

 private:
  Facade* model_;
};
}  // namespace viewer

#endif  // CONTROLLER_H
