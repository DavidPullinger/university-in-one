#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include <GL/glew.h>
#include <iostream>
#include <unordered_map>

class ShaderManager {
public:
  ShaderManager(){};

  GLuint loadShaderProgram(const char *vertShaderFilename,
                           const char *fragShaderFilename);

  void addUniform(const char *name, GLuint program);
  void addAttrib(const char *name, GLuint program);

  GLuint getUniform(const char *name, GLuint program);
  GLuint getAttrib(const char *name, GLuint program);

private:
  std::unordered_map<std::string, GLuint> uniformLocs;
  std::unordered_map<std::string, GLuint> attribLocs;

  GLuint loadShader(const char *shaderFilename, GLenum shaderType);
};

#endif
