#include "inputmanager.h"
#include <iostream>

const glm::mat4 ID = glm::mat4(1.0f);
const float ANGLE = 8.0f;

InputManager::InputManager(glm::mat4 *modelMatrix, glm::mat4 *viewMatrix,
                           GLuint modelShader) {
  this->modelMatrix = modelMatrix;
  this->viewMatrix = viewMatrix;
  this->modelShader = modelShader;
  mode = ROTATE;
  axis = X;
  direction = POSITIVE;
  target = MODEL;
  lighting = AMBIENT;
}

void InputManager::transform() {
  switch (mode) {

  case ROTATE: {
    if (target == VIEW) {
      // rotate camera around origin
      glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
      rotation[axis] = 1.0f * direction;
      glm::mat4 transform = glm::rotate(ID, glm::radians(ANGLE), rotation);
      // originally had code to use glm::lookAt, but the up vector was always
      // (0, 1, 0) so the camera would flip halfway through a rotation in the
      // X-axis, and would never rotate in the Z-axis
      *viewMatrix = *viewMatrix * transform;
    } else {
      // rotate model around origin
      glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
      rotation[axis] = 1.0f * direction;
      glm::mat4 transform = glm::rotate(ID, glm::radians(ANGLE), rotation);
      *modelMatrix = transform * *modelMatrix;
    }
    break;
  }
  case SCALE: {
    if (target == VIEW) {
      // can't scale camera
      return;
    } else {
      // scale model along some axis
      glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
      scale[axis] *= (1.0f + 0.1f * direction);
      glm::mat4 transform = glm::scale(ID, scale);
      *modelMatrix = transform * *modelMatrix;
    }
    break;
  }
  };
}

void InputManager::editLighting() {
  glUseProgram(modelShader);
  std::string lightingCoefficient;
  switch (lighting) {
  case AMBIENT:
    lightingCoefficient = "ka";
    break;
  case DIFFUSE:
    lightingCoefficient = "kd";
    break;
  case SPECULAR:
    lightingCoefficient = "ks";
    break;
  case SHININESS:
    lightingCoefficient = "shininess";
    break;
  }
  // increment or decrement the lighting coefficient
  float loc = glGetUniformLocation(modelShader, lightingCoefficient.c_str());
  float value;
  glGetUniformfv(modelShader, loc, &value);
  if (lighting == SHININESS) {
    value += 1.0f * direction;
  } else {
    value += 0.1f * direction;
  }
  value = glm::max(value, 0.01f);
  glUniform1f(loc, value);
  glUseProgram(0);
}

void InputManager::increment() {
  direction = POSITIVE;
  if (target == LIGHT) {
    editLighting();
  } else {
    transform();
  }
}

void InputManager::decrement() {
  direction = NEGATIVE;
  if (target == LIGHT) {
    editLighting();
  } else {
    transform();
  }
}

void InputManager::setTransformMode(TRANSFORM_MODE mode) { this->mode = mode; }

void InputManager::setAxis(AXIS axis) { this->axis = axis; }

void InputManager::setTarget(TARGET target) { this->target = target; }

void InputManager::setLighting(LIGHTING lighting) { this->lighting = lighting; }

std::string InputManager::getTransformMode() {
  switch (mode) {
  case ROTATE:
    return "Rotate";
  case SCALE:
    return "Scale";
  }
}

std::string InputManager::getTarget() {
  switch (target) {
  case MODEL:
    return "Model";
  case VIEW:
    return "View";
  case LIGHT:
    return "Light";
  }
}

std::string InputManager::getAxis() {
  switch (axis) {
  case X:
    return "X";
  case Y:
    return "Y";
  case Z:
    return "Z";
  }
}

std::string InputManager::getLighting() {
  switch (lighting) {
  case AMBIENT:
    return "Ambient";
  case DIFFUSE:
    return "Diffuse";
  case SPECULAR:
    return "Specular";
  case SHININESS:
    return "Shininess";
  }
}
