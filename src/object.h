//
// Created by rene on 12/06/23.
//
#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include "SDL.h"
#include "fastSineCosine.h"

typedef struct Point
{
  float x;
  float y;
} Point;

class Line {
public:
  enum PointsOrientation { poCollinear = 0, poCCW, poCW};

  Line(const Point& p0, const Point& p1);
  Point P0() const { return p0; }
  Point P1() const { return p1; }
  PointsOrientation orientation(const Point& r) const;
  bool onSegment(const Point& r) const;
  bool intersect(const Line& line) const;

private:
  Point p0;
  Point p1;
};

class Object {
public:
  Object(Point position, float rotationAngle, float directionAngle, float speed, float rotationSpeed,
         float mass = 1, float frictionFactor = 1, float rotationFrictionFactor = 2, bool checkDistanceTraveled = false);

  static void setScreenDimensions(float width, float height);
  void addPoints(std::vector<Point> &pts);

  void setPosition(Point newPosition) { position = newPosition; }
  void setRotationAngle(float newRotationAngle);
  void setDirectionAngle(float directionAngle);
  void setSpeed(float newSpeed);
  void setRotationSpeed(float newRotationSpeed);
  void setDestroyed(bool newDestroyed) { destroyed = newDestroyed; }
  void setAccelerationForce(float newAccelerationForce) { accelerationForce = newAccelerationForce; }
  void setRotationForce(float newRotationForce) { rotationForce = newRotationForce; }

  Point getPosition() const { return position; }
  float getRotationAngle() const { return rotationAngle; }
  float getDirectionAngle() const { return directionAngle; }
  float getSpeed() const { return speed; }
  float getRotationSpeed() const { return rotationSpeed; }
  float getDistanceTraveled() const { return distanceTraveled; }
  bool getDestroyed() const { return destroyed; }

  std::vector<Point> &getRawPoints() {return rawPoints; }
  const std::vector<Point> &getRotatedPoints() const { return rotatedPts; }
  const std::vector<Point> &getTranslatedPoints() const { return translatedPts; }

  bool isInside(Point point) const;
  bool isInside(const Object &object) const;
  bool collision(const Object &object) const;
  bool isVisible();

  void setSpeed();
  void rotatePoints();
  void translatePoints();
  virtual void move(float timeDelta);
  void translationMovement(float timeDelta);
  void rotationalMovement(float timeDelta);
  void wrapAround();
  static float wrapAngle(float angle);
  virtual std::string toString();

protected:
  static Sine sine;
  static Cosine cosine;

private:
  static float screenWidth;
  static float screenHeight;

  Point position;
  float rotationAngle{0};
  float directionAngle{0};
  float mass{0};
  float frictionFactor{0};
  float accelerationForce{0};
  float speed{0};
  float xSpeed{0};
  float ySpeed{0};
  bool checkDistanceTraveled{false};
  float distanceTraveled{0};
  float maxDistanceTraveled{0};
  bool destroyed{false};
  float rotationSpeed{0};
  float rotationForce{0};
  float rotationFrictionFactor{0};

  std::vector<Point> rawPoints;
  std::vector<Point> rotatedPts;
  std::vector<Point> translatedPts;
};

#endif //OBJECT_H

#pragma clang diagnostic pop