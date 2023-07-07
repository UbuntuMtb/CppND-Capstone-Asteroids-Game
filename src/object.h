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
  Object(SDL_FPoint position, float rotationAngle, float speed, float direction, float maxSpeed);
  static void setScreenDimensions(float width, float height);
  void setSpeed(float newSpeed);
  void setDirection(float direction);
  void addPoints(std::vector<SDL_FPoint> &pts);
  bool isInside(SDL_FPoint point);
  void rotate(float angle);
  void translatePoints();
  void move (float timeDelta);
  bool isVisible();
  void wrapAround();
  virtual std::string toString();

  const std::vector<SDL_FPoint> &getTranslatedPoints() const { return translatedPts; }
  const std::vector<SDL_Point> &getTranslatedPointsI() const { return translatedPtsI; }
  SDL_FPoint getPosition() const { return position; }
  float getSpeed() const { return speed; }
  float getDirection() const { return direction; }

private:
  std::vector<SDL_FPoint> points;
  std::vector<SDL_FPoint> rotatedPts;
  std::vector<SDL_FPoint> translatedPts;
  std::vector<SDL_Point> translatedPtsI;
  SDL_FPoint position;
  float rotationAngle;
  float speed{0};
  float direction{0};
  float xSpeed{0};
  float ySpeed{0};
  float maxSpeed;

  static float screenWidth;
  static float screenHeight;
};


#endif //OBJECT_H

#pragma clang diagnostic pop