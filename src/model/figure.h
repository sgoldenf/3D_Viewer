#ifndef FIGURE_H
#define FIGURE_H

#include <vector>

namespace viewer {
struct Point {
  float x;
  float y;
  float z;
};

class Vertex {
 public:
  Vertex(Point p = {0, 0, 0}) : position_(p) {}
  ~Vertex() {}

  auto get_position() const -> const Point& { return position_; }

 private:
  Point position_;
};

class Figure {
 public:
  Figure()
      : vertices_(std::vector<Vertex>()),
        indices_(std::vector<unsigned int>()){};
  Figure(const std::vector<Vertex>& v, const std::vector<unsigned int>& i,
         unsigned int f)
      : vertices_(v), indices_(i), facets_count_(f) {}
  Figure(std::vector<Vertex>&& v, std::vector<unsigned int>&& i, unsigned int f)
      : facets_count_(f) {
    std::swap(vertices_, v);
    std::swap(indices_, i);
  }
  ~Figure() {}

  auto get_vertices() const -> std::vector<Vertex> { return vertices_; }
  auto get_indices() const -> std::vector<unsigned int> { return indices_; }
  auto get_facets_count() const -> unsigned int { return facets_count_; }
  auto get_data() const -> const float* {
    return &(vertices_[0].get_position().x);
  }

 private:
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
  unsigned int facets_count_ = 0;
};
}  // namespace viewer

#endif  // FIGURE_H
