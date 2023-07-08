//
// Created by rene on 26/06/23.
//

#ifndef ASTEROID_H
#define ASTEROID_H

#include "object.h"

class Asteroid: public Object {
public:
  Asteroid(SDL_FPoint position, float speed, int directionAngle, float maxSpeed);
  std::string toString() override;
private:
};

class Ship: public Object {
public:
  Ship(SDL_FPoint position, float speed, int directionAngle, float maxSpeed);
  Ship() : Ship({0, 0}, 0, 0, 0) {}
  std::string toString() override;
private:
};

class Bullet: public Object {
public:
  Bullet(SDL_FPoint position, float speed, int directionAngle, float maxSpeed);
  std::string toString() override;
};

#endif //ASTEROID_H
