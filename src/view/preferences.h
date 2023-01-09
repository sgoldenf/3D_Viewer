#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QColor>

namespace viewer {
struct EdgePreferences {
  enum EdgeType { kSolid, kDashed };
  EdgePreferences(EdgeType type, double thickness, QColor color)
      : type_(type), thickness_(thickness), color_(color){};

  EdgePreferences() : type_(kSolid), thickness_(1), color_(QColor("#F2A348")){};

  EdgeType type_;
  double thickness_;
  QColor color_;
};

struct VertexPreferences {
  enum VertexType { kNone, kCircle, kSquare };
  VertexPreferences(VertexType type, double size, QColor color)
      : type_(type), size_(size), color_(color){};

  VertexPreferences() : type_(kNone), size_(10), color_(QColor("#EE8131")){};

  VertexType type_;
  double size_;
  QColor color_;
};

struct Preferences {
  enum ProjectionType { kCentral, kParallel };
  Preferences(ProjectionType projection_type,
              EdgePreferences::EdgeType edge_type, double edge_thickness,
              QColor edge_color, VertexPreferences::VertexType vertex_type,
              double vertex_size, QColor vertex_color, QColor background_color)
      : projection_type_(projection_type),
        edge_(EdgePreferences(edge_type, edge_thickness, edge_color)),
        vertex_(VertexPreferences(vertex_type, vertex_size, vertex_color)),
        background_color_(background_color) {}

  Preferences()
      : projection_type_(kCentral),
        edge_(EdgePreferences()),
        vertex_(VertexPreferences()),
        background_color_(QColor("#383838")) {}

  ProjectionType projection_type_;
  EdgePreferences edge_;
  VertexPreferences vertex_;
  QColor background_color_;
};
}  // namespace viewer
#endif  // PREFERENCES_H
