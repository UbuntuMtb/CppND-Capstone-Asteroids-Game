#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height, float maxSpeed)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      maxSpeed(maxSpeed) {
  PlaceFood();

  objects.emplace_back(new Asteroid({500, 320}, 50.0, -3 * M_PI / 4, maxSpeed));
  objects.emplace_back(new Asteroid({500, 320}, 50.0, -3 * M_PI / 4, maxSpeed));
  objects.emplace_back(new Asteroid({300, 200}, 60.0, M_PI / 4, maxSpeed));
  objects.emplace_back(new Asteroid({200, 400}, 60.0, M_PI / 2, maxSpeed));

  pShip = new Ship({300, 300}, 0.0, -M_PI / 2, maxSpeed);
  objects.emplace_back(pShip);

//  asteroids.emplace_back(SDL_FPoint{500, 320}, 50.0, -3 * M_PI / 4, maxSpeed);
//  asteroids.emplace_back(SDL_FPoint{300, 200}, 60.0, M_PI / 4, maxSpeed);
//  asteroids.emplace_back(SDL_FPoint{200, 200}, 60.0, 0, maxSpeed);
//  asteroids.emplace_back(SDL_FPoint{200, 400}, 60.0, M_PI / 2, maxSpeed);
//  ship = Ship({300, 300}, 0.0, -M_PI / 2, maxSpeed);

//  for (Asteroid &asteroid: asteroids)
//    objects.push_back(&asteroid);
//  objects.push_back(&ship);
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  float secs_per_frame = (float) target_frame_duration / 1000.0F;
  bool fire = false;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, *pShip, fire);
    Update(secs_per_frame, fire);
    renderer.Render(snake, food, objects);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update(float secs_per_frame, bool &fire) {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }

  for (auto &pObject: objects) {
    if (!pObject->isVisible())
      pObject->wrapAround();

    auto *pAsteroid = dynamic_cast<Asteroid*>(pObject.get());
    if (pAsteroid != nullptr)
      pObject->setRotationAngle(pObject->getRotationAngle() - (float) M_PI / 72);

    pObject->move(secs_per_frame);
  }

  if (fire) {
    auto *bullet = new Bullet(pShip->getPosition(), maxSpeed, pShip->getRotationAngle(), maxSpeed);
    objects.emplace_back(bullet);
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }