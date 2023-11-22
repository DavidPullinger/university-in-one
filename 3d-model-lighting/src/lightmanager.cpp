#include "lightmanager.h"
#include "shadermanager.h"

LightManager::LightManager() {
  // could make this prettier by reading from a file
  Light l;
  l.ambientColor = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
  l.diffuseColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
  l.specularColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
  l.position = glm::vec4(0.0f);
  lights.push_back(l);
  Light l2;
  l2.ambientColor = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
  l2.diffuseColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
  l2.specularColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
  l2.position = glm::vec4(0.0f);
  lights.push_back(l2);
  Light l3;
  l3.ambientColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  l3.diffuseColor = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
  l3.specularColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  l3.position = glm::vec4(0.0f);
  lights.push_back(l3);
}

void LightManager::updateLightPositions(GLuint shader, glm::mat4 &view,
                                        float radius, float time) {
  glUseProgram(shader);
  // set position of lights
  // the first two lights are moving, the third is stationary
  lights[0].position =
      glm::vec4(radius * cos(time), radius * pow(sin(time / 2), 2),
                radius * sin(time), 1.0f);
  lights[1].position = glm::vec4(-lights[0].position.x, lights[0].position.y,
                                 -lights[0].position.z, 1.0f);
  lights[2].position = glm::vec4(0.0f, radius, 0.0f, 1.0f);
  // set position uniforms
  for (int i = 0; i < lights.size(); i++) {
    std::string light = "lights[" + std::to_string(i) + "].";
    // position -> camera space
    glUniform4fv(glGetUniformLocation(shader, (light + "position").c_str()), 1,
                 &(view * lights[i].position)[0]);
  }
}

void LightManager::updateLightUniforms(GLuint shader, glm::mat4 &view) {
  glUseProgram(shader);
  for (int i = 0; i < lights.size(); i++) {
    std::string light = "lights[" + std::to_string(i) + "].";
    // position -> camera space
    glUniform4fv(glGetUniformLocation(shader, (light + "position").c_str()), 1,
                 &(view * lights[i].position)[0]);
    // colors
    glUniform4fv(glGetUniformLocation(shader, (light + "ambientColor").c_str()),
                 1, &lights[i].ambientColor[0]);
    glUniform4fv(glGetUniformLocation(shader, (light + "diffuseColor").c_str()),
                 1, &lights[i].diffuseColor[0]);
    glUniform4fv(
        glGetUniformLocation(shader, (light + "specularColor").c_str()), 1,
        &lights[i].specularColor[0]);
  }
  // set lighting coefficients
  glUniform1f(glGetUniformLocation(shader, "shininess"), 132.0f);
  glUniform1f(glGetUniformLocation(shader, "ka"), 0.3f);
  glUniform1f(glGetUniformLocation(shader, "kd"), 2.0f);
  glUniform1f(glGetUniformLocation(shader, "ks"), 0.8f);
}
