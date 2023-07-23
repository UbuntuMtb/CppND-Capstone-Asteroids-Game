#include "controller.h"
#include <iostream>
#include <iomanip>
#include "SDL.h"

void Controller::HandleInput(bool &running, Ship *pShip, bool &fire) const {
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
          if (pShip) {
            pShip->setDirectionAngle(pShip->getRotationAngle());
            pShip->setAccelerationForce(200);
          }
          break;

        case SDLK_DOWN:
          break;

        case SDLK_LEFT:
          if (pShip)
            pShip->setRotationForce(-720);
            //pShip->setRotationSpeed(-180);
          break;

        case SDLK_RIGHT:
          if (pShip)
            pShip->setRotationForce(720);
            //pShip->setRotationSpeed(180);
          break;

        case SDLK_SPACE:
          fire = true;
          break;
      }
    }
    else if (e.type == SDL_KEYUP) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          if (pShip)
            pShip->setAccelerationForce(0);
          break;
        case SDLK_LEFT:
        case SDLK_RIGHT:
          if (pShip)
            pShip->setRotationForce(0);
            //pShip->setRotationSpeed(0);
          break;
      }
    }
    else {
      //std::cout << std::hex << e.type << std::endl;
    }
  }

  if (pShip)
    std::cout << std::fixed << std::setprecision(1) << std::setfill(' ')
    << std::setw(6) << pShip->getSpeed() << " "
    << std::setw(6) << pShip->getDirectionAngle() << " "
    << std::setw(6) << pShip->getRotationAngle() << " "
    << std::setw(6) << pShip->getRotationSpeed() << std::endl;
}