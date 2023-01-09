#ifndef PREFERENCESMODEL_H
#define PREFERENCESMODEL_H

#include <QObject>
#include <QtCore>

#include "preferences.h"

namespace viewer {
class PreferencesModel;

class PreferencesMemento {
 public:
  explicit PreferencesMemento(Preferences preferences)
      : preferences_(preferences){};

 private:
  friend class PreferencesModel;
  Preferences preferences_;
};

class PreferencesModel : public QObject {
  Q_OBJECT
 public:
  explicit PreferencesModel(QObject *parent = nullptr);
  ~PreferencesModel();
  auto ReadPreferences() -> void;
  auto WritePreferences() -> void;
  auto CreatePreferencesMemento() -> PreferencesMemento *;
  auto ReinstatePreferencesMemento(PreferencesMemento *mem) -> void;
  auto get_preferences() -> Preferences { return preferences_; };
  auto set_preferences(const Preferences &preferences) -> void;

 signals:
  auto PreferencesChanged(Preferences preferences) -> void;

 private:
  Preferences preferences_;

  auto get_preferences_object() -> QJsonObject;
  auto ReadEdgePreferences(const QJsonObject &preferences_object) -> void;
  auto ReadVertexPreferences(const QJsonObject &preferences_object) -> void;
  auto ReadProjectionType(const QJsonObject &preferences_object) -> void;
  auto ReadBackgroundColor(const QJsonObject &preferences_object) -> void;
  auto OpenPreferencesFile() -> QJsonDocument;
};
}  // namespace viewer
#endif  // PREFERENCESMODEL_H
