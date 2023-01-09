#include "controller.h"

viewer::Controller::Controller() : model_(new Facade) {}

viewer::Controller::~Controller() { delete model_; }

auto viewer::Controller::ParseFigure(const std::string& path)
    -> const viewer::Figure& {
  return model_->LoadFigure(path);
}

auto viewer::Controller::Rotate(double x, double y, double z) -> void {
  model_->RotateFigure(x, y, z);
}

auto viewer::Controller::Translate(double x, double y, double z) -> void {
  model_->TranslateFigure(x, y, z);
}

auto viewer::Controller::Scale(double x, double y, double z) -> void {
  model_->ScaleFigure(x, y, z);
}

auto viewer::Controller::GetTransformMatrix() -> QMatrix4x4 {
  return model_->CalculateTransformMatrix();
}

auto viewer::Controller::get_preferences() -> Preferences {
  return model_->get_preferences();
}

auto viewer::Controller::set_preferences(const Preferences& preferences)
    -> void {
  model_->set_preferences(preferences);
}

auto viewer::Controller::SavePreferences() -> PreferencesMemento* {
  return model_->SavePreferences();
}

auto viewer::Controller::ReinstatePreferences(PreferencesMemento* memento)
    -> void {
  model_->ReinstatePreferences(memento);
}
