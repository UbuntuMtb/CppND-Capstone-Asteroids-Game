#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
//
// Created by rene on 12/06/23.
//

#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include "SDL.h"
#include "fastSineCosine.h"

class Line {
public:
  enum PointsOrientation { poCollinear = 0, poCCW, poCW};

  Line(const SDL_FPoint& p0, const SDL_FPoint& p1);
  SDL_FPoint P0() const { return p0; }
  SDL_FPoint P1() const { return p1; }
  PointsOrientation orientation(const SDL_FPoint& r) const;
  bool onSegment(const SDL_FPoint& r) const;
  bool intersect(const Line& line) const;

private:
  SDL_FPoint p0;
  SDL_FPoint p1;
};

class Object {
public:
  Object(SDL_FPoint position, float rotationAngle, float directionAngle,
         float speed, float maxSpeed, float rotationSpeed, float maxRotationSpeed,
         bool checkDistanceTraveled = false);

  static void setScreenDimensions(float width, float height);
  void addPoints(std::vector<SDL_FPoint> &pts);

  void setRotationAngle(float newRotationAngle);
  void setSpeed(float newSpeed);
  void setDirectionAngle(float directionAngle);
  void setDestroyed(bool newDestroyed) { destroyed = newDestroyed; }
  void setRotationSpeed(float newRotationSpeed);

  SDL_FPoint getPosition() const { return position; }
  float getRotationAngle() const { return rotationAngle; }
  float getSpeed() const { return speed; }
  float getDirectionAngle() const { return directionAngle; }
  float getDistanceTraveled() const { return distanceTraveled; }
  bool getDestroyed() const { return destroyed; }
  float getRotationSpeed() const { return rotationSpeed; }
  std::vector<SDL_FPoint > &getPoints() {return points; }
  const std::vector<SDL_FPoint> &getRotatedPoints() const { return rotatedPts; }
  const std::vector<SDL_FPoint> &getTranslatedPoints() const { return translatedPts; }

  bool isInside(SDL_FPoint point);
  bool isVisible();

  void rotatePoints();
  void translatePoints();
  void move (float timeDelta);
  void wrapAround();
  static float wrapAngle(float angle);
  virtual std::string toString();

private:
  static float screenWidth;
  static float screenHeight;
  static Sine sine;
  static Cosine cosine;

  SDL_FPoint position;
  float rotationAngle{0};
  float speed{0};
  float directionAngle{0};
  float maxSpeed;
  float xSpeed{0};
  float ySpeed{0};
  bool checkDistanceTraveled{false};
  float distanceTraveled{0};
  float maxDistanceTraveled{0};
  bool destroyed{false};
  float rotationSpeed{0};
  float maxRotationSpeed{0};

  std::vector<SDL_FPoint> points;
  std::vector<SDL_FPoint> rotatedPts;
  std::vector<SDL_FPoint> translatedPts;
};

#endif //OBJECT_H

#pragma clang diagnostic pop