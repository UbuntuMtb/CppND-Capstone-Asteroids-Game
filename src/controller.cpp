#include "controller.h"
#include <iostream>
#include "SDL.h"

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
          ship.setDirectionAngle(ship.getRotationAngle());
          ship.setSpeed(ship.getSpeed() + 25);
          break;

        case SDLK_DOWN:
          break;

        case SDLK_LEFT:
          ship.setRotationSpeed(ship.getRotationSpeed() - 36);
          //ship.setRotationAngle(ship.getRotationAngle() - 5);
          break;

        case SDLK_RIGHT:
          ship.setRotationSpeed(ship.getRotationSpeed() + 36);
          //ship.setRotationAngle(ship.getRotationAngle() + 5);
          break;

        case SDLK_SPACE:
          fire = true;
          break;
      }
    }
  }
  if (!keyPressed) {
    if (ship.getSpeed() > 0)
      ship.setSpeed(ship.getSpeed() - 7);

    if (ship.getRotationSpeed() > 0) {
      ship.setRotationSpeed(ship.getRotationSpeed() - 36);
      if (ship.getRotationSpeed() < 0)
        ship.setRotationSpeed(0);
    }
    else if (ship.getRotationSpeed() < 0) {
      ship.setRotationSpeed(ship.getRotationSpeed() + 36);
      if (ship.getRotationSpeed() > 0)
        ship.setRotationSpeed(0);
    }
  }
}