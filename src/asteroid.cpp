//
// Created by rene on 26/06/23.
//

#include "asteroid.h"

Asteroid::Asteroid(SDL_FPoint position, float directionAngle,
                   float speed, float rotationSpeed,
                   std::mt19937 engine)
  : Object(position, 0, directionAngle, speed, rotationSpeed, 1, 0, 0, false)
{
  std::vector<SDL_FPoint> points;
  std::uniform_int_distribution<int> randR(15, 50);

  for (int angle = 0; angle < 360; angle += 360 / 10) {
    auto radius = (float) randR(engine);
    SDL_FPoint point = {radius * cosine((float) angle), radius * sine((float) angle)};
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
  for (auto &point: getPoints())
    point = {point.x * factor, point.y * factor};
}

Ship::Ship(SDL_FPoint position, float directionAngle,
           float speed, float rotationSpeed,
           float mass, float frictionFactor, float rotationFrictionFactor)
  : Object(position, directionAngle, directionAngle, speed, rotationSpeed, mass, frictionFactor, rotationFrictionFactor)
{
  std::vector<SDL_FPoint> points = {{15, 0}, {-15, 10}, {-15, -10}};
  addPoints(points);
}

std::string Ship::toString()
{
  return "Ship, " + Object::toString();
}

Bullet::Bullet(SDL_FPoint position, float directionAngle, float speed)
  : Object (position, 0, directionAngle, speed, 0, 1, 0, 0, true) {
  std::vector<SDL_FPoint> points = {{2, 2}, {-2, 2}, {-2, -2}, {2, -2}};
  addPoints(points);
}

std::string Bullet::toString()
{
  return "Bullet, " + Object::toString();
}
