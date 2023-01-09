#include "preferences_model.h"

namespace viewer {
static const QString kPreferencesPath =
    QDir::homePath() + "/.3DViewer_preferences.json";
PreferencesModel::PreferencesModel(QObject *parent) : QObject{parent} {
  ReadPreferences();
}

PreferencesModel::~PreferencesModel() {}

auto PreferencesModel::ReadPreferences() -> void {
  QJsonDocument preferences_json_document = OpenPreferencesFile();

  if (preferences_json_document.isObject()) {
    QJsonObject preferences_object = preferences_json_document.object();
    ReadProjectionType(preferences_object);
    ReadEdgePreferences(preferences_object);
    ReadVertexPreferences(preferences_object);
    ReadBackgroundColor(preferences_object);
  }
}

auto PreferencesModel::WritePreferences() -> void {
  QJsonDocument preferences_json_document(get_preferences_object());
  QByteArray bytes = preferences_json_document.toJson(QJsonDocument::Indented);
  QFile file(kPreferencesPath);
  if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
    QTextStream input_stream(&file);
    input_stream << bytes;
    file.close();
  } else {
    qDebug() << "file open failed: " << kPreferencesPath;
  }
}

auto PreferencesModel::CreatePreferencesMemento() -> PreferencesMemento * {
  return new PreferencesMemento(preferences_);
}

auto PreferencesModel::ReinstatePreferencesMemento(PreferencesMemento *memento)
    -> void {
  preferences_ = memento->preferences_;
}

auto PreferencesModel::set_preferences(const Preferences &preferences) -> void {
  preferences_ = preferences;
  emit PreferencesChanged(preferences_);
}

auto PreferencesModel::get_preferences_object() -> QJsonObject {
  return QJsonObject{
      {"projectionType", preferences_.projection_type_},
      {"edge",
       QJsonObject{
           {"type", preferences_.edge_.type_},
           {"thickness", preferences_.edge_.thickness_},
           {"color", preferences_.edge_.color_.name()},
       }},
      {"vertex",
       QJsonObject{
           {"type", preferences_.vertex_.type_},
           {"size", preferences_.vertex_.size_},
           {"color", preferences_.vertex_.color_.name()},
       }},
      {"backgroundColor", preferences_.background_color_.name()}};
}

auto PreferencesModel::ReadEdgePreferences(
    const QJsonObject &preferences_object) -> void {
  QJsonObject edges = preferences_object["edge"].toObject();
  preferences_.edge_.type_ = static_cast<EdgePreferences::EdgeType>(
      edges["type"].toInt(Preferences().edge_.type_));
  preferences_.edge_.thickness_ = static_cast<double>(
      edges["thickness"].toDouble(Preferences().edge_.thickness_));
  preferences_.edge_.color_ =
      QColor(edges["color"].toString(Preferences().edge_.color_.name()));
}

auto PreferencesModel::ReadVertexPreferences(
    const QJsonObject &preferences_object) -> void {
  QJsonObject vertices = preferences_object["vertex"].toObject();
  preferences_.vertex_.type_ = static_cast<VertexPreferences::VertexType>(
      vertices["type"].toInt(Preferences().vertex_.type_));
  preferences_.vertex_.size_ = static_cast<double>(
      vertices["size"].toDouble(Preferences().vertex_.size_));
  preferences_.vertex_.color_ =
      QColor(vertices["color"].toString(Preferences().vertex_.color_.name()));
}

auto PreferencesModel::ReadProjectionType(const QJsonObject &preferences_object)
    -> void {
  preferences_.projection_type_ = static_cast<Preferences::ProjectionType>(
      preferences_object["projectionType"].toInt(
          Preferences().projection_type_));
}

auto PreferencesModel::ReadBackgroundColor(
    const QJsonObject &preferences_object) -> void {
  preferences_.background_color_ =
      QColor(preferences_object["backgroundColor"].toString(
          Preferences().background_color_.name()));
}

auto PreferencesModel::OpenPreferencesFile() -> QJsonDocument {
  QJsonDocument preferences_json_document;
  QFile file(kPreferencesPath);
  if (file.open(QIODevice::ReadOnly)) {
    QByteArray bytes = file.readAll();
    file.close();

    QJsonParseError error;
    preferences_json_document = QJsonDocument::fromJson(bytes, &error);

    if (error.error != QJsonParseError::NoError) {
      preferences_ = Preferences();
    }
  }
  return preferences_json_document;
}
}  // namespace viewer
