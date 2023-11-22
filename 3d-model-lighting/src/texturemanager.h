#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "GL/glew.h"
#include "shadermanager.h"
#include <string>
#include <vector>

class TextureManager {
public:
  GLuint loadTexture(std::string file, GLuint shader, int textureUnit);
};
#endif
