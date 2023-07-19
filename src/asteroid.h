//
// Created by rene on 26/06/23.
//

#ifndef ASTEROID_H
#define ASTEROID_H

#include <random>
#include "object.h"

class Asteroid: public Object {
public:
  Asteroid(SDL_FPoint position, float directionAngle,
           float speed, /*float maxSpeed,*/ float rotationSpeed/*, float maxRotationSpeed*/);

  void setDirectionAngle(float newDirectionAngle);
  void setGeneration(int newGeneration) { generation = newGeneration; }
  [[nodiscard]] int getGeneration() const { return generation; }

  std::string toString() override;
  void resize(float factor);
private:
  int generation{1};
};

class Ship: public Object {
public:
  Ship(SDL_FPoint position, float directionAngle, float speed, /*float maxSpeed,*/ float rotationSpeed/*, float maxRotationSpeed*/,
       float mass, float frictionFactor);
  Ship() : Ship({0, 0}, 0, 0, 0, 1, 1) {}
  std::string toString() override;
private:
};

class Bullet: public Object {
public:
  Bullet(SDL_FPoint position, float directionAngle, float speed/*, float maxSpeed*/);
  std::string toString() override;
};

#endif //ASTEROID_H
