#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "asteroid.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, float maxSpeed);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const {  return score; }

 private:
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  //std::vector<Asteroid> asteroids;
  Ship *pShip{nullptr};
  //std::vector<Object*> objects;
  std::vector<std::unique_ptr<Object>> objects;
  float maxSpeed;

  void Update(float secs_per_frame, bool &fire);
};

#endif