#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include "SDL.h"
#include "asteroid.h"

class Renderer {
 public:
  Renderer(std::size_t screen_width, std::size_t screen_height);
  ~Renderer();

  void Render(std::vector<std::unique_ptr<Object>> &objects);
  void UpdateWindowTitle(int level, int lives, int score, int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
};

#endif