#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "object.h"

int main() {

  constexpr std::size_t kFramesPerSecond{20};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr float kMaxSpeed{100};
  constexpr float kAsteroidsCount{4};

  Object::setScreenDimensions(kScreenWidth, kScreenHeight);
  Renderer renderer(kScreenWidth, kScreenHeight);
  Controller controller;
  Game game(kScreenWidth, kScreenHeight, kMaxSpeed, kAsteroidsCount);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  return 0;
}