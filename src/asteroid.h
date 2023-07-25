//
// Created by rene on 26/06/23.
//

#ifndef ASTEROID_H
#define ASTEROID_H

#include <random>
#include "object.h"

class Asteroid: public Object {
public:
  Asteroid(Point position, float directionAngle, float speed, float rotationSpeed,
           std::mt19937 engine);
  void setGeneration(int newGeneration) { generation = newGeneration; }
  int getGeneration() const { return generation; }

  std::string toString() override;
  void resize(float factor);
private:
  int generation{1};
};

class Ship: public Object {
public:
  Ship(Point position, float directionAngle, float speed, float rotationSpeed,
       float mass, float frictionFactor, float rotationFrictionFactor);
  Ship() : Ship({0, 0}, 0, 0, 0, 1, 1, 1) {}

  void setGhost(bool newGhost);
  bool getGhost() const { return ghost; }
  void move(float timeDelta) override;
  std::string toString() override;
private:
  bool ghost{false};
  Uint32 ghostTimeout{0};
};

class Bullet: public Object {
public:
  Bullet(Point position, float directionAngle, float speed);
  std::string toString() override;
};

class Dust: public Object {
public:
  Dust(Point position, float directionAngle, float speed);
  std::string toString() override;
};

#endif //ASTEROID_H
