#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

struct FaceData {
  int vertexIndex[3];
  int texCoordIndex[3];
  int normalIndex[3];
};

class GeometryData {
public:
  void loadFromOBJFile(std::string filename);

  int vertexCount();

  glm::vec3 center();
  void boundingBox(glm::vec3 &min, glm::vec3 &max);

  void *vertexData();
  void *textureCoordData();
  void *normalData();
  void *tangentData();
  void *bitangentData();

private:
  std::vector<float> vertices;
  std::vector<float> textureCoords;
  std::vector<float> normals;
  std::vector<float> tangents;
  std::vector<float> bitangents;

  std::vector<FaceData> faces;
};

#endif
