#include "color_picker_button.h"

namespace viewer {
ColorPickerButton::ColorPickerButton(QWidget* parent)
    : QPushButton(parent),
      color_(QColor("255, 255, 255")),
      dialog_(new QColorDialog(color_, parentWidget())) {
  setStyleSheet(
      "margin-left: 3px; border: 1px solid gray; border-radius: 3; "
      "background-color: rgb(255, 255, 255)");
  connect(this, &ColorPickerButton::clicked, this,
          &ColorPickerButton::ChangeColor);
  connect(dialog_, &QColorDialog::colorSelected, this,
          &ColorPickerButton::set_color);
}

ColorPickerButton::~ColorPickerButton() { delete dialog_; }

auto ColorPickerButton::UpdateColor() -> void {
  setStyleSheet(
      "margin-left: 3px; border: 1px solid gray; border-radius: 3; "
      "background-color: " +
      color_.name());
  emit ColorChanged();
}

auto ColorPickerButton::ChangeColor() -> void {
  dialog_->setCurrentColor(color_);
  dialog_->open();
}

auto ColorPickerButton::set_color(const QColor& new_color) -> void {
  if (new_color != color_) {
    this->color_ = new_color;
  }
  UpdateColor();
}

auto ColorPickerButton::get_color() const -> const QColor& { return color_; }
}  // namespace viewer
