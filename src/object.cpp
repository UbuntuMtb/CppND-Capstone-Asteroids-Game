//
// Created by rene on 12/06/23.
//

#include "object.h"
#include <cmath>
#include <stdexcept>

float Object::screenWidth = 0;
float Object::screenHeight = 0;

bool operator==(const SDL_FPoint& p0, const SDL_FPoint& p1)
{
  return p0.x == p1.x && p0.y == p1.y;
}

Line::Line(const SDL_FPoint& p0, const SDL_FPoint& p1) : p0(p0), p1(p1)
{
  if (p1 == p0)
    throw std::invalid_argument("Line: Points are equal!");
}

bool Line::onSegment(const SDL_FPoint& r) const
{
  float maxX = std::max(p0.x, p1.x);
  float minX = std::min(p0.x, p1.x);
  float maxY = std::max(p0.y, p1.y);
  float minY = std::max(p0.y, p1.y);

  return minX <= r.x && r.x <= maxX && minY <= r.y && r.y <= maxY;
}

PointsOrientation Line::orientation(const SDL_FPoint& r) const
{
  float value = (r.y - p1.y) * (p1.x - p0.x) - (p1.y - p0.y) * (r.x - p1.x);
  if (value == 0.0)
    return poCollinear;
  else if (value > 0.0)
    return poCCW;
  else
    return poCW;
}

//
// Based o n https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
//
bool Line::intersect(const Line& line) const
{
  PointsOrientation o1 = orientation(line.P0());
  PointsOrientation o2 = orientation(line.P1());
  PointsOrientation o3 = line.orientation(p0);
  PointsOrientation o4 = line.orientation(p1);

  if (o1 != o2 && o3 != o4)
    return true;

  if (o1 == poCollinear && onSegment(line.P0()))
    return true;

  if (o2 == poCollinear && onSegment(line.P1()))
    return true;

  if (o3 == poCollinear && line.onSegment(p0))
    return true;

  if (o4 == poCollinear && line.onSegment(p1))
    return true;

  return false;
}

Object::Object(SDL_FPoint position, float rotationAngle, float speed, float directionAngle, float maxSpeed)
  : position(position), rotationAngle(rotationAngle), maxSpeed(maxSpeed)
{
  if (maxSpeed < 0.0)
    throw std::range_error("Negative max speed!");
  setSpeed(speed);
  setDirectionAngle(directionAngle);
}

void Object::setScreenDimensions(float width, float height)
{
  screenWidth = width;
  screenHeight = height;
}

void Object::addPoints(std::vector<SDL_FPoint> &pts)
{
  points = pts;
  rotatePoints();
  translatePoints();
}

//
// Based on: https://www.geeksforgeeks.org/2d-transformation-rotation-objects/
//
void Object::setRotationAngle(float newRotationAngle) {
  if (newRotationAngle == rotationAngle)
    return;

  rotationAngle = newRotationAngle;
  rotatePoints();
}

void Object::setSpeed(float newSpeed)
{
  if (newSpeed == speed)
    return;

  if (newSpeed < 0.0)
    speed = 0;
  else if (newSpeed > maxSpeed)
    speed = maxSpeed;
  else
    speed = newSpeed;

  xSpeed = speed * cos(directionAngle);
  ySpeed = speed * sin(directionAngle);
}

void Object::setDirectionAngle(float newDirectionAngle)
{
  if (newDirectionAngle == directionAngle)
    return;

  float TWO_PI = 2 * M_PI;
  if (newDirectionAngle > TWO_PI || newDirectionAngle < -TWO_PI)
    directionAngle = fmod(newDirectionAngle, TWO_PI);
  else
    directionAngle = newDirectionAngle;

  xSpeed = speed * cos(directionAngle);
  ySpeed = speed * sin(directionAngle);
}

//
// Based on https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
//
bool Object::isInside(SDL_FPoint point)
{
  if (translatedPts.size() < 3)
    return false;

  Line extendedLine(point, {100000.0, point.y});
  int count = 0;
  auto n = translatedPts.size();

  for (int i = 0; i < n; i++)
  {
    Line side(translatedPts[i], translatedPts[(i + 1) % n]);

    if (side.intersect(extendedLine))
    {
      if (side.orientation(point) == poCollinear)
        return side.onSegment(point);
      count++;
    }
  }
  return count & 1;
}

bool Object::isVisible() {
  for (const SDL_FPoint &point: translatedPts) {
    if (0 <= point.x && point.x < screenWidth && 0 < point.y && point.y < screenHeight)
      return true;
  }
  return false;
}

void Object::rotatePoints()
{
  float cosA = cos(rotationAngle);
  float sinA = sin(rotationAngle);
  rotatedPts.clear();

  for (const SDL_FPoint &pt: points) {
    SDL_FPoint rotatedPt {pt.x * cosA - pt.y * sinA, pt.x * sinA + pt.y * cosA};
    rotatedPts.emplace_back(rotatedPt);
  }
}

void Object::translatePoints()
{
  translatedPts.clear();
  translatedPtsI.clear();

  for (const SDL_FPoint &point: rotatedPts)
    translatedPts.push_back({point.x + position.x, point.y + position.y});

  for (const SDL_FPoint &point: translatedPts)
    translatedPtsI.push_back({(int) point.x, (int) point.y});
}

void Object::move(float timeDelta)
{
  position = {position.x + xSpeed * timeDelta, position.y + ySpeed * timeDelta};
  translatePoints();
}

void Object::wrapAround() {
  bool wrapPosX = true;
  bool wrapNegX = true;
  bool wrapPosY = true;
  bool wrapNegY = true;

  for (SDL_FPoint &point: translatedPts) {
    if (point.x >= 0)
      wrapNegX = false;
    if (point.x < screenWidth)
      wrapPosX = false;
    if (point.y >= 0)
      wrapNegY = false;
    if (point.y < screenHeight)
      wrapPosY = false;
  }

  if (wrapPosX) {
    float xMax = 0;
    for (SDL_FPoint &point: rotatedPts)
      if (point.x > xMax)
        xMax = point.x;
    position.x = -xMax;
  }
  else if (wrapNegX) {
    float xMin = 0;
    for (SDL_FPoint &point: rotatedPts)
      if (point.x < xMin)
        xMin = point.x;
    position.x = screenWidth - xMin;
  }

  if (wrapPosY) {
    float yMax = 0;
    for (SDL_FPoint &point: rotatedPts)
      if (point.y > yMax)
        yMax = point.y;
    position.y = -yMax;
  }
  else if (wrapNegY) {
    float yMin = 0;
    for (SDL_FPoint &point: rotatedPts)
      if (point.y < yMin)
        yMin = point.y;
    position.y = screenHeight - yMin;
  }

  translatePoints();
}

std::string Object::toString() {
  return "[" + std::to_string(position.x) + ", " + std::to_string(position.y) + "], " +
    "rotation: " + std::to_string(rotationAngle) + ", speed: " + std::to_string(speed) + "], " +
    "direction: " + std::to_string(directionAngle);
}