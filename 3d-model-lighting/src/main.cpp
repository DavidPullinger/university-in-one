#include "SDL.h"
#include "glwindow.h"

int main(int argc, char **argv) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Error",
                             "Unable to initialize SDL", 0);
    return 1;
  }

  OpenGLWindow window(argv[1], argv[2], argv[3]);

  bool running = true;
  while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = false;
      } else if (!window.handleEvent(e)) {
        running = false;
      }
    }
    window.render();

    SDL_Delay(10);
  }

  window.cleanup();
  SDL_Quit();
  return 0;
}
