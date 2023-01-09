#ifndef FILE_READER_H
#define FILE_READER_H

#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>

#include "figure.h"

namespace viewer {
class FileReader {
 public:
  FileReader() {}
  ~FileReader() {}

  auto ReadFigure(const std::string& path) -> const viewer::Figure&;

 private:
  auto CountVerticesAndIndices(size_t& vertices_count, size_t& indices_count,
                               size_t& indices_lines, FILE* file) -> void;
  size_t CountSpaces(const char* string);
  auto ParseFile(std::vector<Vertex>& vertices,
                 std::vector<unsigned int>& indices, FILE* file) -> void;
  auto ParseVertex(std::vector<Vertex>& vertices, const char* line, int pos)
      -> void;
  auto ParseIndices(std::vector<unsigned int>& indices, const char* line,
                    int& pos) -> void;

  static constexpr size_t kBufferSize = 512;
  viewer::Figure figure_;
};
}  // namespace viewer

#endif  // FILE_READER_H
