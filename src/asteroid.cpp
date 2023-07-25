//
// Created by rene on 26/06/23.
//
#include "SDL.h"
#include "asteroid.h"
#include <iostream>

//////////////////////////////////////
// Asteroid Object
//////////////////////////////////////

Asteroid::Asteroid(Point position, float directionAngle,
                   float speed, float rotationSpeed,
                   std::mt19937 engine)
  : Object(position, 0, directionAngle, speed, rotationSpeed, 1, 0, 0, false)
{
  std::vector<Point> points;
  std::uniform_int_distribution<int> randR(15, 50);

  for (int angle = 0; angle < 360; angle += 360 / 10) {
    auto radius = (float) randR(engine);
    Point point = {radius * cosine((float) angle), radius * sine((float) angle)};
    points.emplace_back(point);
  }
  addPoints(points);
}

std::string Asteroid::toString()
{
  return "Asteroid, " + Object::toString();
}

void Asteroid::resize(float factor)
{
  for (auto &point: getRawPoints())
    point = {point.x * factor, point.y * factor};
}

//////////////////////////////////////
// Ship Object
//////////////////////////////////////

Ship::Ship(Point position, float directionAngle,
           float speed, float rotationSpeed,
           float mass, float frictionFactor, float rotationFrictionFactor)
  : Object(position, directionAngle, directionAngle, speed, rotationSpeed, mass, frictionFactor, rotationFrictionFactor)
{
  std::vector<Point> points = {{15, 0}, {-15, 10}, {-15, -10}};
  addPoints(points);
  setGhost(true);
}

void Ship::setGhost(bool newGhost)
{
  if (newGhost == ghost)
    return;
  ghost = newGhost;
  if (ghost)
    ghostTimeout = SDL_GetTicks() + 4000;
  // std::cout << "Ghost:" << ghost << std::endl;
}

void Ship::move(float timeDelta)
{
  Object::move(timeDelta);
  if (getGhost() && SDL_GetTicks() > ghostTimeout)
    setGhost(false);
}

std::string Ship::toString()
{
  return "Ship, " + Object::toString();
}

Bullet::Bullet(Point position, float directionAngle, float speed)
  : Object (position, 0, directionAngle, speed, 0, 1, 0, 0, true) {
  std::vector<Point> points = {{2, 2}, {-2, 2}, {-2, -2}, {2, -2}};
  addPoints(points);
}

std::string Bullet::toString()
{
  return "Bullet, " + Object::toString();
}

//////////////////////////////////////
// Dust Object
//////////////////////////////////////
Dust::Dust(Point position, float directionAngle, float speed)
  : Object(position, 0, directionAngle, speed, 0, 1, 0, 0, true)
{
  std::vector<Point> points = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  addPoints(points);
}

std::string Dust::toString() {
  return Object::toString();
}
