#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include <GL/glew.h>
#include <SDL.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "inputmanager.h"
#include "lightmanager.h"
#include "shadermanager.h"
#include "texturemanager.h"
#include "windowmanager.cpp"

class OpenGLWindow {
public:
  OpenGLWindow(std::string modelName, std::string albedoMapName,
               std::string normalMapName);

  void initModelVBO(std::string name);
  void initSkyboxVBO();
  void render();
  bool handleEvent(SDL_Event e);
  void cleanup();
  void initMVP();
  void updateMVP();

private:
  SDL_Window *sdlWin;
  InputManager inputmanager;
  LightManager lightmanager;
  ShaderManager shadermanager;

  GLuint modelShader;
  GLuint skyboxShader;
  GLuint modelVAO;
  GLuint modelVBO;
  GLuint skyboxVAO;
  GLuint skyboxVBO;
  float time;
  bool useNormalMap;

  int numVerticesModel;
  int numVerticesSkybox;
  float distToObj;        // for placement of camera and lights
  glm::vec3 objectCenter; // for resetting model
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;
};

#endif
