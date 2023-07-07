//
// Created by rene on 26/06/23.
//

#include "asteroid.h"

Asteroid::Asteroid(SDL_FPoint position, float speed, float direction, float maxSpeed)
  : Object(position, 0, speed, direction, maxSpeed)
{
  std::vector<SDL_FPoint> points = {{20, 20}, {20, -20}, {-20, -20}, {-20, 20}};
  addPoints(points);
}

std::string Asteroid::toString()
{
  return "Asteroid";
}

Ship::Ship(SDL_FPoint position, float speed, float direction, float maxSpeed)
  : Object(position, -M_PI / 2, speed, direction, maxSpeed)
{
  std::vector<SDL_FPoint> points = {{15, 0}, {-15, 10}, {-15, -10}};
  addPoints(points);
}

std::string Ship::toString()
{
  return "Ship";
}

Bullet::Bullet(SDL_FPoint position, float speed, float direction, float maxSpeed)
  : Object (position, 0, speed, direction, maxSpeed) {
  std::vector<SDL_FPoint> points = {{2, 2}, {-2, 2}, {-2, -2}, {2, -2}};
  addPoints(points);
}

std::string Bullet::toString()
{
  return "Bullet";
}
