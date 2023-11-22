#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>

enum TRANSFORM_MODE { ROTATE, SCALE };
enum AXIS { X, Y, Z };
enum TARGET { MODEL, VIEW, LIGHT };
enum DIRECTION { POSITIVE = 1, NEGATIVE = -1 };
enum LIGHTING { AMBIENT, DIFFUSE, SPECULAR, SHININESS };

class InputManager {
public:
  InputManager(){};
  InputManager(glm::mat4 *modelMatrix, glm::mat4 *viewMatrix,
               GLuint modelShader);

  void increment();
  void decrement();

  void setTransformMode(TRANSFORM_MODE mode);
  void setAxis(AXIS axis);
  void setTarget(TARGET target);
  void setLighting(LIGHTING lighting);

  std::string getTransformMode();
  std::string getAxis();
  std::string getTarget();
  std::string getLighting();

private:
  TRANSFORM_MODE mode;
  AXIS axis;
  DIRECTION direction;
  TARGET target;
  LIGHTING lighting;

  GLuint modelShader;
  glm::mat4 *modelMatrix;
  glm::mat4 *viewMatrix;

  void transform();
  void editLighting();
};

#endif
