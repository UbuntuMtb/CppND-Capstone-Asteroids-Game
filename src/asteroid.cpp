//
// Created by rene on 26/06/23.
//

#include "asteroid.h"

Asteroid::Asteroid(SDL_FPoint position, float directionAngle,
                   float speed, float rotationSpeed)
  : Object(position, 0, directionAngle, speed, rotationSpeed, 1, 0, false)
{
  std::vector<SDL_FPoint> points = {{20, 20}, {20, -20}, {-20, -20}, {-20, 20}};
  addPoints(points);
}

std::string Asteroid::toString()
{
  return "Asteroid, " + Object::toString();
}

void Asteroid::resize(float factor)
{
  for (auto &point: getPoints())
    point = {point.x * factor, point.y * factor};
}

void Asteroid::setDirectionAngle(float newDirectionAngle) {
  Object::setDirectionAngle(newDirectionAngle);
  setSpeed(getSpeed());
}

Ship::Ship(SDL_FPoint position, float directionAngle,
           float speed, float rotationSpeed,
           float mass, float frictionFactor)
  : Object(position, directionAngle, directionAngle, speed, rotationSpeed, mass, frictionFactor)
{
  std::vector<SDL_FPoint> points = {{15, 0}, {-15, 10}, {-15, -10}};
  addPoints(points);
}

std::string Ship::toString()
{
  return "Ship, " + Object::toString();
}

Bullet::Bullet(SDL_FPoint position, float directionAngle, float speed)
  : Object (position, 0, directionAngle, speed, 0, 1, 0, true) {
  std::vector<SDL_FPoint> points = {{2, 2}, {-2, 2}, {-2, -2}, {2, -2}};
  addPoints(points);
}

std::string Bullet::toString()
{
  return "Bullet, " + Object::toString();
}
