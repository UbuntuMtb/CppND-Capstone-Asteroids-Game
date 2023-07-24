#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(std::size_t screen_width, std::size_t screen_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, (int) screen_width,
                                (int) screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(std::vector<std::unique_ptr<Object>> &objects) {
  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render objects
  for (auto &pObject: objects) {
    auto pShip = dynamic_cast<Ship*>(pObject.get());
    auto pASteroid = dynamic_cast<Asteroid*>(pObject.get());
    auto pBullet = dynamic_cast<Bullet*>(pObject.get());
    auto pDust = dynamic_cast<Dust*>(pObject.get());

    if (pShip)
    {
      if (pShip->getGhost()) {
        if (SDL_GetTicks() > flashTimeout) {
          flash = !flash;
          flashTimeout = SDL_GetTicks() + 250;
        }
        // Flash ship
        if (flash)
          SDL_SetRenderDrawColor(sdl_renderer, 0x33, 0xFF, 0xFF, 0xFF);
        else
          SDL_SetRenderDrawColor(sdl_renderer, 0x19, 0x77, 0x77, 0xFF);
      }
      else
        SDL_SetRenderDrawColor(sdl_renderer, 0x33, 0xFF, 0xFF, 0xFF);
    }
    else if (pASteroid)
      SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x99, 0xFF);
    else if (pBullet)
      SDL_SetRenderDrawColor(sdl_renderer, 0x33, 0xFF, 0xFF, 0xFF);
    else if (pDust)
      SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    auto &points = pObject->getTranslatedPoints();

    for (int i = 0; i < points.size(); i++) {
      const SDL_FPoint &pt0 = points[i];
      const SDL_FPoint &pt1 = points[(i + 1) % points.size()];
      SDL_RenderDrawLine(sdl_renderer, (int) pt0.x, (int) pt0.y, (int) pt1.x, (int) pt1.y);
    }
  }

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int level, int lives, int score, int fps) {
  std::string title{"Asteroids Level: " + std::to_string(level) + " Lives: "};
  title += lives > 0 ? std::string(lives, 'A') : "Game Over!";
  title += " Score: " + std::to_string(score) + " FPS: " + std::to_string(fps);
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
