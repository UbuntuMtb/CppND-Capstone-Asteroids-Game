//
// Created by rene on 12/06/23.
//
#include "object.h"
#include <stdexcept>
#include <cmath>

float Object::screenWidth = 0;
float Object::screenHeight = 0;
Sine Object::sine;
Cosine Object::cosine;

bool operator==(const Point& p0, const Point& p1)
{
  return p0.x == p1.x && p0.y == p1.y;
}

Line::Line(const Point& p0, const Point& p1) : p0(p0), p1(p1)
{
  if (p1 == p0)
    throw std::invalid_argument("Line: Points are equal!");
}

bool Line::onSegment(const Point& r) const
{
  float maxX = std::max(p0.x, p1.x);
  float minX = std::min(p0.x, p1.x);
  float maxY = std::max(p0.y, p1.y);
  float minY = std::max(p0.y, p1.y);

  return minX <= r.x && r.x <= maxX && minY <= r.y && r.y <= maxY;
}

Line::PointsOrientation Line::orientation(const Point& r) const
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
// Based on https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
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

Object::Object(Point position, float rotationAngle, float directionAngle,
               float speed, float rotationSpeed,
               float mass, float frictionFactor, float rotationFrictionFactor,
               bool checkDistanceTraveled)
  : position(position), rotationSpeed(rotationSpeed),
    mass(mass), frictionFactor(frictionFactor), rotationFrictionFactor(rotationFrictionFactor),
    checkDistanceTraveled(checkDistanceTraveled)
{
  //if (maxSpeed < 0.0)
  //  throw std::range_error("Negative max speed!");
  setRotationAngle(rotationAngle);
  setDirectionAngle(directionAngle);
  setSpeed(speed);

  if (screenWidth <= 0 || screenHeight <= 0)
    throw std::range_error("Object::screenWidth or Object::screenHeight have not been set!");
  maxDistanceTraveled = fmin(screenWidth, screenHeight) * 3 / 4;
}

void Object::setScreenDimensions(float width, float height)
{
  screenWidth = width;
  screenHeight = height;
}

void Object::addPoints(std::vector<Point> &pts)
{
  rawPoints = pts;
  rotatedPts.resize(rawPoints.size());
  translatedPts.resize(rawPoints.size());
  //translatedPtsI.resize(points.size());
  rotatePoints();
  translatePoints();
}

//
// Based on: https://www.geeksforgeeks.org/2d-transformation-rotation-objects/
//
void Object::setRotationAngle(float newRotationAngle) {
  if (newRotationAngle == rotationAngle)
    return;
  rotationAngle = wrapAngle(newRotationAngle);
  rotatePoints();
}

void Object::setDirectionAngle(float newDirectionAngle)
{
  if (newDirectionAngle == directionAngle)
    return;
  directionAngle = wrapAngle(newDirectionAngle);
}

void Object::setSpeed(float newSpeed)
{
  //if (newSpeed == speed)
  //  return;
  if (newSpeed < 0.0)
    speed = 0;
  else
    speed = newSpeed;

  //xSpeed = speed * cosine(directionAngle);
  //ySpeed = speed * sine(directionAngle);
  setSpeed();
}

void Object::setRotationSpeed(float newRotationSpeed)
{
  if (newRotationSpeed == rotationSpeed)
    return;
  rotationSpeed = newRotationSpeed;
}

void Object::setSpeed()
{
  xSpeed = speed * cosine(directionAngle);
  ySpeed = speed * sine(directionAngle);
}

//
// Based on https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
//
bool Object::isInside(Point point) const
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
      if (side.orientation(point) == Line::poCollinear)
        return side.onSegment(point);
      count++;
    }
  }
  return count & 1;
}

bool Object::isInside(const Object &object) const
{
  for (auto point: object.getTranslatedPoints())
    if (isInside(point))
      return true;
  return false;
}

bool Object::collision(const Object &object) const
{
  if (isInside(object))
    return true;
  if (object.isInside(*this))
    return true;
  return false;
}

bool Object::isVisible() {
  for (const Point &point: translatedPts) {
    if (0 <= point.x && point.x < screenWidth && 0 < point.y && point.y < screenHeight)
      return true;
  }
  return false;
}

void Object::rotatePoints()
{
  float cosA = cosine(rotationAngle);
  float sinA = sine(rotationAngle);

  for (int i = 0; i < rawPoints.size(); i++) {
    Point &pt = rawPoints[i];
    rotatedPts[i] = {pt.x * cosA - pt.y * sinA, pt.x * sinA + pt.y * cosA};
  }
}

void Object::translatePoints()
{
  for (int i = 0; i < rotatedPts.size(); i++) {
    Point &point = rotatedPts[i];
    translatedPts[i] = Point{point.x + position.x, point.y + position.y};
  }
}

void Object::move(float timeDelta)
{
  //float newRotationAngle = getRotationAngle() + getRotationSpeed() * timeDelta;
  //setRotationAngle(newRotationAngle);
  rotationalMovement(timeDelta);
  translationMovement(timeDelta);
  translatePoints();

  if (checkDistanceTraveled) {
    distanceTraveled += speed * timeDelta;
    if (distanceTraveled > maxDistanceTraveled)
      destroyed = true;
  }
}

// Uniformly accelerated particle movement calculations
// https://www.profesorenlinea.cl/fisica/Movimiento_rectilineo_acelerado.html
// https://www.toptal.com/game/video-game-physics-part-i-an-introduction-to-rigid-body-dynamics
void Object::translationMovement(float timeDelta)
{
  float xFrictionForce = frictionFactor * xSpeed;
  float yFrictionForce = frictionFactor * ySpeed;
  float xAccForce = accelerationForce * cosine(directionAngle);
  float yAccForce = accelerationForce * sine(directionAngle);
  float xNetForce = xAccForce - xFrictionForce;
  float yNetForce = yAccForce - yFrictionForce;
  float xAccel = xNetForce / mass;
  float yAccel = yNetForce / mass;

  position.x += xSpeed * timeDelta + xAccel * timeDelta * timeDelta / 2;
  position.y += ySpeed * timeDelta + yAccel * timeDelta * timeDelta / 2;
  xSpeed += xAccel * timeDelta;
  ySpeed += yAccel * timeDelta;

  speed = sqrt(xSpeed * xSpeed + ySpeed * ySpeed);
  if (speed < 10) {
    xSpeed = 0;
    ySpeed = 0;
  }
}

// Uniformly accelerated particle rotation calculations
// http://19e37.com/wiki/index.php/Movimiento_Circular_Uniformemente_Acelerado_-_MCUA
void Object::rotationalMovement(float timeDelta)
{
  float rotationFrictionForce = rotationFrictionFactor * rotationSpeed;
  float netRotationForce = rotationForce - rotationFrictionForce;
  float angularAcceleration = netRotationForce / mass;

  float newRotationAngle = getRotationAngle() + getRotationSpeed() * timeDelta + angularAcceleration * timeDelta * timeDelta / 2;
  setRotationAngle(newRotationAngle);
  setRotationSpeed(getRotationSpeed() + angularAcceleration * timeDelta);

  if (-10 < getRotationSpeed() && getRotationSpeed() < 10 && rotationForce == 0)
    setRotationSpeed(0);
}

void Object::wrapAround() {
  bool wrapPosX = true;
  bool wrapNegX = true;
  bool wrapPosY = true;
  bool wrapNegY = true;

  for (Point &point: translatedPts) {
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
    for (Point &point: rotatedPts)
      if (point.x > xMax)
        xMax = point.x;
    position.x = -xMax;
  }
  else if (wrapNegX) {
    float xMin = 0;
    for (Point &point: rotatedPts)
      if (point.x < xMin)
        xMin = point.x;
    position.x = screenWidth - xMin;
  }

  if (wrapPosY) {
    float yMax = 0;
    for (Point &point: rotatedPts)
      if (point.y > yMax)
        yMax = point.y;
    position.y = -yMax;
  }
  else if (wrapNegY) {
    float yMin = 0;
    for (Point &point: rotatedPts)
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

float Object::wrapAngle(float angle) {
  if (angle >= 360 || angle <= -360)
    return fmod(angle, (float) 360);
  else
    return angle;
}
