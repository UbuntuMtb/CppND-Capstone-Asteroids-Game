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

enum PointsOrientation { poCollinear = 0, poCCW, poCW};

class Line {
public:
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
  Object(SDL_FPoint position, float rotationAngle, float speed, float directionAngle, float maxSpeed);

  static void setScreenDimensions(float width, float height);
  void addPoints(std::vector<SDL_FPoint> &pts);

  void setRotationAngle(float newRotationAngle);
  void setSpeed(float newSpeed);
  void setDirectionAngle(float direction);

  SDL_FPoint getPosition() const { return position; }
  float getRotationAngle() const { return rotationAngle; }
  float getSpeed() const { return speed; }
  float getDirectionAngle() const { return directionAngle; }
  const std::vector<SDL_FPoint> &getRotatedPoints() const { return rotatedPts; }
  const std::vector<SDL_FPoint> &getTranslatedPoints() const { return translatedPts; }
  const std::vector<SDL_Point> &getTranslatedPointsI() const { return translatedPtsI; }

  bool isInside(SDL_FPoint point);
  bool isVisible();

  void rotatePoints();
  void translatePoints();
  void move (float timeDelta);
  void wrapAround();
  virtual std::string toString();

private:
  static float screenWidth;
  static float screenHeight;

  SDL_FPoint position;
  float rotationAngle;
  float speed{0};
  float directionAngle{0};
  float maxSpeed;
  float xSpeed{0};
  float ySpeed{0};

  std::vector<SDL_FPoint> points;
  std::vector<SDL_FPoint> rotatedPts;
  std::vector<SDL_FPoint> translatedPts;
  std::vector<SDL_Point> translatedPtsI;
};

#endif //OBJECT_H

#pragma clang diagnostic pop