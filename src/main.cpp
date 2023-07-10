#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "object.h"

int main() {

  constexpr std::size_t kFramesPerSecond{30};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};
  constexpr float kMaxSpeed{200};

  SDL_FPoint p0 = {1, 1}, p1 = {10, 2};
  SDL_FPoint q0 = {1, 2,}, q1 = {10, 3};
  Line(p0, p1).intersect(Line(q0, q1)) ? std::cout << "Yes\n" : std::cout << "No\n";

  p0 = {10, 0}, p1 = {0, 10};
  q0 = {0, 0}, q1 = {10, 0};
  //q0 = {0, 0}, q1 = {5, 5};
  Line(p0, p1).intersect(Line(q0, q1)) ? std::cout << "Yes\n" : std::cout << "No\n";

  p0 = {-5, -5}, p1 = {0, 0};
  q0 = {1, 1}, q1 = {10, 10};
  Line(p0, p1).intersect(Line(q0, q1)) ? std::cout << "Yes\n" : std::cout << "No\n";

  std::vector<SDL_FPoint> points = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};
  Object::setScreenDimensions(kScreenWidth, kScreenHeight);
  Object object({0, 0}, 0, 0, 0, 100, 0, 0);
  object.addPoints(points);
  SDL_FPoint point = {5, 5};

  if (object.isInside(point))
    std::cout << "Point is inside.\n";
  else
    std::cout << "Point is outside.\n";

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight, kMaxSpeed);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  return 0;
}