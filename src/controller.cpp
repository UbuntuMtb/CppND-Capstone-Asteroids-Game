#include "controller.h"
#include <iostream>
#include "SDL.h"

void Controller::HandleInput(bool &running, Ship &ship, bool &fire) const {
  SDL_Event e;
  bool keyPressed = false;
  static Uint32 currentTime;
  static Uint32 speedUpTimeout;
  static Uint32 speedDwTimeout;
  static bool doSpeedDw = false;

  fire = false;
  currentTime = SDL_GetTicks();

  while (SDL_PollEvent(&e))
  {
    keyPressed = true;
    if (e.type == SDL_QUIT) {
      running = false;
      //std::cout << std::hex << e.type << std::endl;
    }
    else if (e.type == SDL_KEYDOWN) {
      //std::cout << std::hex << e.type << " " << e.key.keysym.sym << std::endl;
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ship.setDirectionAngle(ship.getRotationAngle());
          ship.setAccelerationForce(200);
          break;

        case SDLK_DOWN:
          break;

        case SDLK_LEFT:
          ship.setRotationSpeed(-180);
          break;

        case SDLK_RIGHT:
          ship.setRotationSpeed(180);
          break;

        case SDLK_SPACE:
          fire = true;
          break;
      }
    }
    else if (e.type == SDL_KEYUP) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ship.setAccelerationForce(0);
          break;
        case SDLK_LEFT:
        case SDLK_RIGHT:
          ship.setRotationSpeed(0);
          break;
      }
    }
    else {
      //std::cout << std::hex << e.type << std::endl;
    }
  }

  std::cout << ship.getSpeed() << " " << ship.getDirectionAngle() << " " << ship.getRotationAngle() << std::endl;
  //std::cout << ship.getRotationSpeed() << std::endl;
}