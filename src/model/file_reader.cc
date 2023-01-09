#include "file_reader.h"

#include <array>

auto viewer::FileReader::ReadFigure(const std::string& path)
    -> const viewer::Figure& {
  FILE* file = std::fopen(path.c_str(), "r");
  if (!file) {
    throw std::ios_base::failure("Failed to open the file: " + path + "\n");
  }
  size_t vertices_count = 0;
  size_t indices_count = 0;
  size_t indices_lines = 0;
  CountVerticesAndIndices(vertices_count, indices_count, indices_lines, file);
  std::rewind(file);
  std::vector<Vertex> vertices(vertices_count);
  std::vector<unsigned int> indices(indices_count);
  ParseFile(vertices, indices, file);
  std::fclose(file);
  figure_ = Figure(std::move(vertices), std::move(indices), indices_lines);
  return figure_;
}

auto viewer::FileReader::CountVerticesAndIndices(size_t& vertices_count,
                                                 size_t& indices_count,
                                                 size_t& indices_lines,
                                                 FILE* file) -> void {
  char line[kBufferSize];
  while (std::fgets(line, sizeof(line), file)) {
    if (line[0] == 'v' && line[1] == ' ') {
      ++vertices_count;
    }
    if (line[0] == 'f' && line[1] == ' ') {
      indices_count += CountSpaces(line) * 2;
      ++indices_lines;
    }
  }
}

auto viewer::FileReader::CountSpaces(const char* string) -> size_t {
  size_t count = 0;
  const char* p = string;
  while (*p) {
    if (*p == ' ') {
      ++count;
    }
    ++p;
  }
  return count;
}

auto viewer::FileReader::ParseFile(std::vector<Vertex>& vertices,
                                   std::vector<unsigned int>& indices,
                                   FILE* file) -> void {
  char line[kBufferSize];
  int vertices_counter = 0;
  int indices_counter = 0;
  while (std::fgets(line, sizeof(line), file)) {
    if (line[0] == 'v' && line[1] == ' ') {
      ParseVertex(vertices, line, vertices_counter);
      ++vertices_counter;
    }
    if (line[0] == 'f' && line[1] == ' ') {
      ParseIndices(indices, line, indices_counter);
    }
  }
}

auto viewer::FileReader::ParseVertex(std::vector<Vertex>& vertices,
                                     const char* line, int pos) -> void {
  std::array<float, 3> tmp_array;
  int i = 0;
  const char* ptr = line;
  char* p_end;
  while (*ptr && i < 3) {
    tmp_array[i] = strtod(ptr, &p_end);
    if (ptr != p_end) {
      ptr = p_end;
      ++i;
    }
    ++ptr;
  }
  vertices[pos] = viewer::Point{tmp_array[0], tmp_array[1], tmp_array[2]};
}

auto viewer::FileReader::ParseIndices(std::vector<unsigned int>& indices,
                                      const char* line, int& pos) -> void {
  const char* p = line;
  char* p_end;
  int first_index = -1;
  while (*p) {
    if (isdigit(*p)) {
      int num = (std::strtol(p, &p_end, 10)) - 1;
      p = p_end;
      if (first_index == -1) {
        first_index = num;
      }
      indices[pos] = num;
      ++pos;
      if (first_index != num) {
        indices[pos] = num;
        ++pos;
      }
      while (*p != ' ' && *p != '\0') {
        ++p;
      }
    } else {
      ++p;
    }
  }
  if (first_index != -1) {
    indices[pos] = first_index;
    ++pos;
  }
}
