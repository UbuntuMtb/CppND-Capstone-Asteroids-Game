#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

void Controller::HandleInput(bool &running, Ship &ship, bool &fire) const {
  SDL_Event e;
  bool keyPressed = false;
  fire = false;

  while (SDL_PollEvent(&e)) {
    keyPressed = true;
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ship.setSpeed(ship.getSpeed() + 3);
          break;

        case SDLK_DOWN:
          break;

        case SDLK_LEFT:
          ship.setDirection(ship.getDirection() - (float) M_PI / 50);
          ship.rotate(-M_PI / 50);
          break;

        case SDLK_RIGHT:
          ship.setDirection(ship.getDirection() + (float) M_PI / 50);
          ship.rotate(M_PI / 50);
          break;

        case SDLK_SPACE:
          fire = true;
          break;
      }
    }
  }
  if (!keyPressed)
    ship.setSpeed(ship.getSpeed() - 5);
}