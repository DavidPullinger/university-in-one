#include "GL/glew.h"
#include "SDL.h"
#include "stdio.h"
#include <iostream>
#include <string>

class WindowUtils {
private:
  static const char *glGetErrorString(GLenum error) {
    switch (error) {
    case GL_NO_ERROR:
      return "GL_NO_ERROR";
    case GL_INVALID_ENUM:
      return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
      return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
      return "GL_INVALID_OPERATION";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_OUT_OF_MEMORY:
      return "GL_OUT_OF_MEMORY";
    default:
      return "UNRECOGNIZED";
    }
  }

public:
  static void glPrintError(const char *label = "Unlabelled Error Checkpoint",
                           bool alwaysPrint = false) {
    GLenum error = glGetError();
    if (alwaysPrint || (error != GL_NO_ERROR)) {
      printf("%s: OpenGL error flag is %s\n", label, glGetErrorString(error));
    }
  }

  static SDL_Window *initGL(std::string modelName) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Window *sdlWin =
        SDL_CreateWindow("OpenGL Prac 2", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    if (!sdlWin) {
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Error",
                               "Unable to create window", 0);
    }
    SDL_GLContext glc = SDL_GL_CreateContext(sdlWin);
    SDL_GL_MakeCurrent(sdlWin, glc);
    SDL_GL_SetSwapInterval(1);

    glewExperimental = true;
    GLenum glewInitResult = glewInit();
    glGetError(); // Consume the error erroneously set by glewInit()
    if (glewInitResult != GLEW_OK) {
      const GLubyte *errorString = glewGetErrorString(glewInitResult);
      std::cout << "Unable to initialize glew: " << errorString;
    }

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    glClearColor(0, 0, 0, 1);

    return sdlWin;
  }
};
