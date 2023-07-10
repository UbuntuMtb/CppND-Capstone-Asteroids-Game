//
// Created by rene on 26/06/23.
//

#include "asteroid.h"

Asteroid::Asteroid(SDL_FPoint position, float speed, int directionAngle, float maxSpeed)
  : Object(position, 0, directionAngle, speed, maxSpeed, 36, 36)
{
  std::vector<SDL_FPoint> points = {{20, 20}, {20, -20}, {-20, -20}, {-20, 20}};
  addPoints(points);
}

std::string Asteroid::toString()
{
  return "Asteroid, " + Object::toString();
}

Ship::Ship(SDL_FPoint position, float speed, int directionAngle, float maxSpeed)
  : Object(position, -90, directionAngle, speed, maxSpeed, 0, 270)
{
  std::vector<SDL_FPoint> points = {{15, 0}, {-15, 10}, {-15, -10}};
  addPoints(points);
}

std::string Ship::toString()
{
  return "Ship, " + Object::toString();
}

Bullet::Bullet(SDL_FPoint position, float speed, int directionAngle, float maxSpeed)
  : Object (position, 0, directionAngle, speed, maxSpeed, 0, 0, true) {
  std::vector<SDL_FPoint> points = {{2, 2}, {-2, 2}, {-2, -2}, {2, -2}};
  addPoints(points);
}

std::string Bullet::toString()
{
  return "Bullet, " + Object::toString();
}
