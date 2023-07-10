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
  Object(SDL_FPoint position, int rotationAngle, int directionAngle, float speed, float maxSpeed, int rotationSpeed, int maxRotationSpeed,
         bool checkDistanceTraveled = false);

  static void setScreenDimensions(float width, float height);
  void addPoints(std::vector<SDL_FPoint> &pts);

  void setRotationAngle(int newRotationAngle);
  void setSpeed(float newSpeed);
  void setDirectionAngle(int directionAngle);
  void setActive(bool newActive) { active = newActive; }
  void setRotationSpeed(int newRotationSpeed);

  SDL_FPoint getPosition() const { return position; }
  int getRotationAngle() const { return rotationAngle; }
  float getSpeed() const { return speed; }
  int getDirectionAngle() const { return directionAngle; }
  float getDistanceTraveled() const { return distanceTraveled; }
  bool getActive() const { return active; }
  int getRotationSpeed() const { return rotationSpeed; }
  const std::vector<SDL_FPoint> &getRotatedPoints() const { return rotatedPts; }
  const std::vector<SDL_FPoint> &getTranslatedPoints() const { return translatedPts; }
  const std::vector<SDL_Point> &getTranslatedPointsI() const { return translatedPtsI; }

  bool isInside(SDL_FPoint point);
  bool isVisible();

  void rotatePoints();
  void translatePoints();
  void move (float timeDelta);
  void wrapAround();
  static int wrapAngle(int angle);
  virtual std::string toString();

private:
  static float screenWidth;
  static float screenHeight;
  static Sine sine;
  static Cosine cosine;

  SDL_FPoint position;
  int rotationAngle{0};
  float speed{0};
  int directionAngle{0};
  float maxSpeed;
  float xSpeed{0};
  float ySpeed{0};
  bool checkDistanceTraveled{false};
  float distanceTraveled{0};
  float maxDistanceTraveled{0};
  bool active{true};
  int rotationSpeed{0};
  int maxRotationSpeed{0};

  std::vector<SDL_FPoint> points;
  std::vector<SDL_FPoint> rotatedPts;
  std::vector<SDL_FPoint> translatedPts;
  std::vector<SDL_Point> translatedPtsI;
};

#endif //OBJECT_H

#pragma clang diagnostic pop