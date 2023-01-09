#include "preferences_widget.h"

#include "ui_preferences_widget.h"

PreferencesWidget::PreferencesWidget(QWidget* parent,
                                     viewer::Controller* controller)
    : QWidget(parent),
      ui(new Ui::PreferencesWidget),
      controller_(controller),
      memento_(nullptr),
      edge_color_button_(new viewer::ColorPickerButton),
      vertex_color_button_(new viewer::ColorPickerButton),
      background_color_button_(new viewer::ColorPickerButton) {
  ui->setupUi(this);

  connect(ui->ok_button, &QPushButton::clicked, this,
          &PreferencesWidget::OnApplyButtonClicked);
  connect(ui->close_button, &QPushButton::clicked, this,
          &PreferencesWidget::OnCloseButtonClicked);
  ui->edges_layout->setWidget(2, QFormLayout::FieldRole, edge_color_button_);
  ui->vertex_layout->setWidget(2, QFormLayout::FieldRole, vertex_color_button_);
  ui->background_layout->setWidget(0, QFormLayout::FieldRole,
                                   background_color_button_);
  RestoreSettingsFromModel();
  ConnectToSaveSettings();
}

PreferencesWidget::~PreferencesWidget() {
  delete ui;
  delete memento_;
}

auto PreferencesWidget::OnApplyButtonClicked() -> void {
  controller_->SavePreferencesToFile();
  delete memento_;
  memento_ = nullptr;
  hide();
}

auto PreferencesWidget::OnCloseButtonClicked() -> void {
  hide();
  if (memento_) {
    controller_->ReinstatePreferences(memento_);
    delete memento_;
    memento_ = nullptr;
  }
  RestoreSettingsFromModel();
}

auto PreferencesWidget::OnSettingChanged() -> void {
  if (!memento_) {
    memento_ = controller_->SavePreferences();
  }
  controller_->set_preferences(BuildPreferencesFromWidgets());
}

auto PreferencesWidget::ConnectToSaveSettings() -> void {
  connect(ui->projection_combobox, &QComboBox::currentTextChanged, this,
          &PreferencesWidget::OnSettingChanged);
  connect(ui->edges_thickness, &QDoubleSpinBox::valueChanged, this,
          &PreferencesWidget::OnSettingChanged);
  connect(ui->vertices_size, &QDoubleSpinBox::valueChanged, this,
          &PreferencesWidget::OnSettingChanged);
  connect(ui->edges_type, &QComboBox::currentTextChanged, this,
          &PreferencesWidget::OnSettingChanged);
  connect(ui->vertices_type, &QComboBox::currentTextChanged, this,
          &PreferencesWidget::OnSettingChanged);
  connect(edge_color_button_, &viewer::ColorPickerButton::ColorChanged, this,
          &PreferencesWidget::OnSettingChanged);
  connect(vertex_color_button_, &viewer::ColorPickerButton::ColorChanged, this,
          &PreferencesWidget::OnSettingChanged);
  connect(background_color_button_, &viewer::ColorPickerButton::ColorChanged,
          this, &PreferencesWidget::OnSettingChanged);
}

auto PreferencesWidget::RestoreSettingsFromModel() -> void {
  viewer::Preferences preferences = controller_->get_preferences();
  ui->projection_combobox->setCurrentIndex(preferences.projection_type_);
  ui->edges_type->setCurrentIndex(preferences.edge_.type_);
  ui->edges_thickness->setValue(preferences.edge_.thickness_);
  edge_color_button_->set_color(preferences.edge_.color_);
  ui->vertices_type->setCurrentIndex(preferences.vertex_.type_);
  ui->vertices_size->setValue(preferences.vertex_.size_);
  vertex_color_button_->set_color(preferences.vertex_.color_);
  background_color_button_->set_color(preferences.background_color_);
}

auto PreferencesWidget::BuildPreferencesFromWidgets() -> viewer::Preferences {
  viewer::Preferences preferences;
  QString projection_string = ui->projection_combobox->currentText();
  if (projection_string == "Central") {
    preferences.projection_type_ = viewer::Preferences::kCentral;
  } else if (projection_string == "Parallel") {
    preferences.projection_type_ = viewer::Preferences::kParallel;
  }
  preferences.edge_ = BuildEdgePreferencesFromWidgets();
  preferences.vertex_ = BuildVertexPreferencesFromWidgets();
  preferences.background_color_ = background_color_button_->get_color();

  return preferences;
}

auto PreferencesWidget::BuildEdgePreferencesFromWidgets()
    -> viewer::EdgePreferences {
  viewer::EdgePreferences edge_preferences;
  QString edge_type_string = ui->edges_type->currentText();
  if (edge_type_string == "Solid") {
    edge_preferences.type_ = viewer::EdgePreferences::kSolid;
  } else if (edge_type_string == "Dashed") {
    edge_preferences.type_ = viewer::EdgePreferences::kDashed;
  }
  edge_preferences.thickness_ = ui->edges_thickness->value();
  edge_preferences.color_ = edge_color_button_->get_color();

  return edge_preferences;
}

auto PreferencesWidget::BuildVertexPreferencesFromWidgets()
    -> viewer::VertexPreferences {
  viewer::VertexPreferences vertex_preferences;
  QString vertex_type_string = ui->vertices_type->currentText();
  if (vertex_type_string == "None") {
    vertex_preferences.type_ = viewer::VertexPreferences::kNone;
  } else if (vertex_type_string == "Square") {
    vertex_preferences.type_ = viewer::VertexPreferences::kSquare;
  } else if (vertex_type_string == "Circle") {
    vertex_preferences.type_ = viewer::VertexPreferences::kCircle;
  }
  vertex_preferences.size_ = ui->vertices_size->value();
  vertex_preferences.color_ = vertex_color_button_->get_color();

  return vertex_preferences;
}
