#include "facade.h"

auto viewer::Facade::LoadFigure(std::string path) -> const viewer::Figure& {
  return file_reader_.ReadFigure(path);
}

auto viewer::Facade::CalculateTransformMatrix() -> QMatrix4x4 {
  matrix::Matrix m(4, 4);
  m.SetToIdentity();
  m *= translation_matrix_builder_.get_transform_matrix()->get_matrix();
  m *= rotation_matrix_builder_.get_transform_matrix()->get_matrix();
  m *= scale_matrix_builder_.get_transform_matrix()->get_matrix();
  QMatrix4x4 result;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result(i, j) = m(i, j);
    }
  }
  return result;
}

auto viewer::Facade::TranslateFigure(double x, double y, double z) -> void {
  translation_matrix_builder_.BuildTransformMatrix(x, y, z);
}

auto viewer::Facade::RotateFigure(double x, double y, double z) -> void {
  rotation_matrix_builder_.BuildTransformMatrix(x, y, z);
}

auto viewer::Facade::ScaleFigure(double x, double y, double z) -> void {
  scale_matrix_builder_.BuildTransformMatrix(x, y, z);
}

auto viewer::Facade::get_preferences() -> Preferences {
  return preferences_->get_preferences();
}

auto viewer::Facade::set_preferences(const Preferences& preferences) -> void {
  preferences_->set_preferences(preferences);
};

auto viewer::Facade::SavePreferences() -> PreferencesMemento* {
  return preferences_->CreatePreferencesMemento();
}

auto viewer::Facade::ReinstatePreferences(PreferencesMemento* memento) -> void {
  preferences_->ReinstatePreferencesMemento(memento);
}
