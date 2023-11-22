#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H
#include "inputmanager.h"
#include "shadermanager.h"
#include <vector>

struct Light {
  glm::vec4 position;
  glm::vec4 ambientColor;
  glm::vec4 diffuseColor;
  glm::vec4 specularColor;
};

class LightManager {
public:
  LightManager();

  void updateLightPositions(GLuint shader, glm::mat4 &view, float radius,
                            float time);
  void updateLightUniforms(GLuint shader, glm::mat4 &view);

private:
  std::vector<Light> lights;
};
#endif
