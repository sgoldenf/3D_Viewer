#include "file_reader.h"
#include "gtest/gtest.h"
#include "transform_matrix.h"

class FileReaderTest : public ::testing::Test {
 protected:
  viewer::FileReader file_reader;

  static void TestVerticesEquality(std::vector<viewer::Vertex> vertices,
                                   std::vector<viewer::Vertex> test);
  static void TestIndicesEquality(std::vector<unsigned int> indices,
                                  std::vector<unsigned int> test);
};

void FileReaderTest::TestVerticesEquality(std::vector<viewer::Vertex> vertices,
                                          std::vector<viewer::Vertex> test) {
  EXPECT_EQ(vertices.size(), test.size());
  std::vector<viewer::Vertex>::iterator i1 = vertices.begin();
  std::vector<viewer::Vertex>::iterator i2 = test.begin();
  while (i2 != test.end()) {
    EXPECT_EQ((*i1).get_position().x, (*i2).get_position().x);
    EXPECT_EQ((*i1).get_position().y, (*i2).get_position().y);
    EXPECT_EQ((*i1).get_position().z, (*i2).get_position().z);
    ++i1, ++i2;
  }
}

void FileReaderTest::TestIndicesEquality(std::vector<unsigned int> indices,
                                         std::vector<unsigned int> test) {
  EXPECT_EQ(indices.size(), test.size());
  for (size_t i = 0; i < test.size(); ++i) EXPECT_EQ(indices[i], test[i]);
}

TEST_F(FileReaderTest, exceptions) {
  EXPECT_ANY_THROW(file_reader.ReadFigure("aboba.obj"));
}

TEST_F(FileReaderTest, cube) {
  viewer::Figure cube = file_reader.ReadFigure("../obj_examples/cube.obj");
  std::vector<viewer::Vertex> test_vertices = {
      viewer::Vertex(viewer::Point{1, 1, -1}),
      viewer::Vertex(viewer::Point{1, -1, -1}),
      viewer::Vertex(viewer::Point{1, 1, 1}),
      viewer::Vertex(viewer::Point{1, -1, 1}),
      viewer::Vertex(viewer::Point{-1, 1, -1}),
      viewer::Vertex(viewer::Point{-1, -1, -1}),
      viewer::Vertex(viewer::Point{-1, 1, 1}),
      viewer::Vertex(viewer::Point{-1, -1, 1})};
  std::vector<unsigned int> test_indices{
      4, 2, 2, 0, 0, 4, 2, 7, 7, 3, 3, 2, 6, 5, 5, 7, 7, 6, 1, 7, 7, 5, 5, 1,
      0, 3, 3, 1, 1, 0, 4, 1, 1, 5, 5, 4, 4, 6, 6, 2, 2, 4, 2, 6, 6, 7, 7, 2,
      6, 4, 4, 5, 5, 6, 1, 3, 3, 7, 7, 1, 0, 2, 2, 3, 3, 0, 4, 0, 0, 1, 1, 4};
  TestVerticesEquality(cube.get_vertices(), test_vertices);
  TestIndicesEquality(cube.get_indices(), test_indices);
}

TEST(AffineTransformationsTest, translation) {
  viewer::Point point{2, 3, 4};
  viewer::TranslationMatrixBuilder builder;
  builder.BuildTransformMatrix(5, 6, 7);
  viewer::TransformMatrix* matrix = builder.get_transform_matrix();
  viewer::Point result = matrix->TransformPoint(point);
  EXPECT_EQ(result.x, 7);
  EXPECT_EQ(result.y, 9);
  EXPECT_EQ(result.z, 11);
}

TEST(AffineTransformationsTest, scale) {
  viewer::Point point{2, 3, 4};
  viewer::ScaleMatrixBuilder builder;
  builder.BuildTransformMatrix(2, 2, 2);
  viewer::TransformMatrix* matrix = builder.get_transform_matrix();
  viewer::Point result = matrix->TransformPoint(point);
  EXPECT_EQ(result.x, 4);
  EXPECT_EQ(result.y, 6);
  EXPECT_EQ(result.z, 8);
}

TEST(AffineTransformationsTest, rotation) {
  viewer::Point point{2, 3, 4};
  viewer::RotationMatrixBuilder builder;
  builder.BuildTransformMatrix(0.523599, 0.785398, 1.0472);
  viewer::TransformMatrix* matrix = builder.get_transform_matrix();
  viewer::Point result = matrix->TransformPoint(point);

  EXPECT_NEAR(result.x, 1.9442222, 1e-7);
  EXPECT_NEAR(result.y, 4.5636659, 1e-7);
  EXPECT_NEAR(result.z, 2.0959372, 1e-7);
}

int main() {
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
