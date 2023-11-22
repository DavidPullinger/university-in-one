#include "shadermanager.h"
#include <GL/glew.h>
#include <iostream>
#include <stdio.h>

GLuint ShaderManager::loadShader(const char *shaderFilename,
                                 GLenum shaderType) {
  FILE *shaderFile = fopen(shaderFilename, "r");
  if (!shaderFile) {
    return 0;
  }

  fseek(shaderFile, 0, SEEK_END);
  long shaderSize = ftell(shaderFile);
  fseek(shaderFile, 0, SEEK_SET);

  char *shaderText = new char[shaderSize + 1];
  size_t readCount = fread(shaderText, 1, shaderSize, shaderFile);
  shaderText[readCount] = '\0';
  fclose(shaderFile);

  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, (const char **)&shaderText, NULL);
  glCompileShader(shader);

  delete[] shaderText;

  return shader;
}

GLuint ShaderManager::loadShaderProgram(const char *vertShaderFilename,
                                        const char *fragShaderFilename) {
  GLuint vertShader = loadShader(vertShaderFilename, GL_VERTEX_SHADER);
  GLuint fragShader = loadShader(fragShaderFilename, GL_FRAGMENT_SHADER);

  GLuint program = glCreateProgram();
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

  GLint linkStatus;
  glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
  if (linkStatus != GL_TRUE) {
    GLsizei logLength = 0;
    GLchar message[1024];
    glGetProgramInfoLog(program, 1024, &logLength, message);
    std::cout << "Shader load error: " << message << std::endl;
  }

  return program;
};

// void ShaderManager::addUniform(const char *name, GLuint program) {
//   GLuint loc = glGetUniformLocation(program, name);
//   if (loc == GL_INVALID_INDEX) {
//     std::cout << "Unable to get location of " << name << std::endl;
//   }
//   uniformLocs[name] = loc;
// }
// void ShaderManager::addAttrib(const char *name, GLuint program) {
//   GLuint loc = glGetAttribLocation(program, name);
//   if (loc == GL_INVALID_INDEX) {
//     std::cout << "Unable to get location of " << name << std::endl;
//   }
//   attribLocs[name] = loc;
// }
//
// GLuint ShaderManager::getUniform(const char *name, GLuint program) {
//   if (uniformLocs.find(name) == uniformLocs.end()) {
//     addUniform(name, program);
//   }
//   return uniformLocs[name];
// }
//
// GLuint ShaderManager::getAttrib(const char *name, GLuint program) {
//   if (attribLocs.find(name) == attribLocs.end()) {
//     addAttrib(name);
//   }
//   return attribLocs[name];
// }
