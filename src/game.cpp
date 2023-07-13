#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t screen_width, std::size_t screen_height, float maxSpeed)
    : screen_width(screen_width), screen_height(screen_height), maxSpeed(maxSpeed),
      engine(dev()), randomDirectionAngle(0, 360)
 {
  objects.emplace_back(new Asteroid({500, 220}, -135, 50, maxSpeed, 90, 90));
  objects.emplace_back(new Asteroid({500, 320}, 135, 50, maxSpeed, 90, 90));
  objects.emplace_back(new Asteroid({300, 200}, 45, 60, maxSpeed, 90, 90));
  objects.emplace_back(new Asteroid({200, 400}, 180, 60, maxSpeed, 90, 90));

  centerPoint = {(float) screen_width/2, (float) screen_height/2};
  pShip = new Ship(centerPoint, 0.0, 0, maxSpeed, 0, 180);
  objects.emplace_back(pShip);
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
    renderer.Render(objects);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(lives, score, frame_count);
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

/*void Game::PlaceFood() {
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
}*/

void Game::Update(float secs_per_frame, bool &fire)
{
  for (auto &pObject: objects) {
    if (!pObject->isVisible())
      pObject->wrapAround();
    pObject->move(secs_per_frame);
  }

  for (auto &pObject: objects) {
    auto pBullet = dynamic_cast<Bullet*>(pObject.get());
    if (pBullet != nullptr && !pBullet->getDestroyed())
    {
      for (auto &pObject2: objects) {
        auto pAsteroid = dynamic_cast<Asteroid*>(pObject2.get());
        if (pAsteroid != nullptr)
        {
          if (pAsteroid->isInside(pBullet->getPosition())) {
            pBullet->setDestroyed(true);
            if (pAsteroid->getGeneration() == 3) {
              pAsteroid->setDestroyed(true);
              switch (pAsteroid->getGeneration()){
                case 1: score += 20; break;
                case 2: score += 50; break;
                case 3: score += 100; break;
              }
            }
            else {
              pAsteroid->setGeneration(pAsteroid->getGeneration() + 1);
              pAsteroid->resize(0.75);
              pAsteroid->setDirectionAngle(pAsteroid->getDirectionAngle() + 90);
              auto pNewAsteroid = new Asteroid(*pAsteroid);
              pNewAsteroid->setDirectionAngle(pNewAsteroid->getDirectionAngle() + 180);
              objects.emplace_back(pNewAsteroid);
            }
            break;
          }
        }
      }
    }
  }

  for (auto &pObject: objects) {
    auto pAsteroid = dynamic_cast<Asteroid*>(pObject.get());
    if (pAsteroid != nullptr)
    if (pShip->isInside((*pAsteroid))) {
      if (lives > 0) {
        lives--;
        pShip->setPosition(centerPoint);
      } else {
        pShip->setDestroyed(true);
      }
      break;
    }
  }

  if (fire) {
    auto *pBullet = new Bullet(pShip->getPosition(), pShip->getRotationAngle(), maxSpeed, maxSpeed);
    objects.emplace_back(pBullet);
  }

  for (auto it = objects.begin(); it != objects.end();) {
    auto &pObject = *it;
    if (pObject->getDestroyed()) {
      auto ptrShip = dynamic_cast<Ship *>(pObject.get());
      if (ptrShip != nullptr)
        pShip = nullptr;
      it = objects.erase(it);
    }
    else
      ++it;
  }
}
