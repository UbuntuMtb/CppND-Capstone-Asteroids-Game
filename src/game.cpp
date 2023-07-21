#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t screen_width, std::size_t screen_height, float maxSpeed, int asteroidCount)
    : screen_width(screen_width), screen_height(screen_height),
      maxSpeed(maxSpeed),
      asteroidCount(asteroidCount),
      engine(dev()),
      randDirection(-180, 180),
      randSpeed((int) maxSpeed / 4, (int) maxSpeed),
      randX(0, (int) screen_width),
      randY(0, (int) screen_height),
      randRotationSpeed(-90, 90)
 {
  for (int i = 0; i < asteroidCount; i++) {
    SDL_FPoint position = {(float) randX(engine), (float) randY(engine)};
    auto pAsteroid = new Asteroid(position, (float) randDirection(engine), (float) randSpeed(engine),
                                  (float) randRotationSpeed(engine), engine);
    objects.emplace_back(pAsteroid);
  }

  centerPoint = {(float) screen_width / 2, (float) screen_height / 2};
  pShip = new Ship(centerPoint, -90.0, 0, 0, 1, 0.5);
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
          if (pBullet->collision(*pAsteroid)) {
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
              pAsteroid->setDirectionAngle(pAsteroid->getDirectionAngle() + (float) randDirection(engine) / 2);
              pAsteroid->setSpeed((float) randSpeed(engine));

              auto pNewAsteroid = new Asteroid(*pAsteroid);
              pNewAsteroid->setDirectionAngle(pAsteroid->getDirectionAngle() + (float) randDirection(engine) / 2);
              pNewAsteroid->setSpeed((float) randSpeed(engine));
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
    if (pShip->collision((*pAsteroid))) {
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
    auto *pBullet = new Bullet(pShip->getPosition(), pShip->getRotationAngle(), 2 * maxSpeed);
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
