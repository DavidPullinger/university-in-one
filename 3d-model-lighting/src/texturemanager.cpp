#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <string>

#include "stb_image.h"
#include "texturemanager.h"

GLuint TextureManager::loadTexture(std::string file, GLuint shader,
                                   int textureUnit) {
  // use specified shader
  glUseProgram(shader);

  // continue with texture loading
  GLuint textureID;
  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glGenTextures(1, &textureID);

  int width, height, numChannels;
  // flip image
  stbi_set_flip_vertically_on_load(true);
  unsigned char *imageData =
      stbi_load(file.c_str(), &width, &height, &numChannels, 0);
  if (!imageData) {
    std::cerr << "Failed to load texture: " << file << std::endl;
  }
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, imageData);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  stbi_image_free(imageData);

  return textureID;
}
