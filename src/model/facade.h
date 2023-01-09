#ifndef FACADE_H
#define FACADE_H

#include <QMatrix4x4>

#include "figure.h"
#include "file_reader.h"
#include "preferences_model.h"
#include "transform_matrix.h"

namespace viewer {
class Facade {
 public:
  Facade() : preferences_(new PreferencesModel()){};
  Facade(const Facade&) = delete;
  Facade& operator=(Facade&&) = delete;
  ~Facade() { delete preferences_; };
  auto LoadFigure(std::string path) -> const Figure&;
  auto CalculateTransformMatrix() -> QMatrix4x4;
  auto TranslateFigure(double x, double y, double z) -> void;
  auto RotateFigure(double x, double y, double z) -> void;
  auto ScaleFigure(double x, double y, double z) -> void;
  auto SavePreferences() -> PreferencesMemento*;
  auto SavePreferencesToFile() -> void { preferences_->WritePreferences(); };
  auto ReinstatePreferences(PreferencesMemento* memento) -> void;
  auto get_preferences() -> Preferences;
  auto set_preferences(const Preferences& preferences) -> void;
  auto get_preferences_model() -> PreferencesModel* { return preferences_; };

 private:
  FileReader file_reader_;
  RotationMatrixBuilder rotation_matrix_builder_;
  TranslationMatrixBuilder translation_matrix_builder_;
  ScaleMatrixBuilder scale_matrix_builder_;
  PreferencesModel* preferences_;
};
}  // namespace viewer

#endif  // FACADE_H
