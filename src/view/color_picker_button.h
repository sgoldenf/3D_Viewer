#ifndef COLORPICKERBUTTON_H
#define COLORPICKERBUTTON_H

#include <QColor>
#include <QColorDialog>
#include <QPushButton>

namespace viewer {
class ColorPickerButton : public QPushButton {
  Q_OBJECT
 public:
  ColorPickerButton(QWidget* parent = nullptr);
  ~ColorPickerButton();
  auto get_color() const -> const QColor&;

 public slots:
  auto ChangeColor() -> void;
  auto set_color(const QColor& new_color) -> void;

 signals:
  auto ColorChanged() -> void;

 private:
  auto UpdateColor() -> void;
  QColor color_{};
  QColorDialog* dialog_;
};
}  // namespace viewer
#endif  // COLORPICKERBUTTON_H
