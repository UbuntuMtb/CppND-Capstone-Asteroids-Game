#include "game.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include "SDL.h"

Game::Game(std::size_t screen_width, std::size_t screen_height, float maxSpeed, int asteroidCount)
    : /*screen_width(screen_width), screen_height(screen_height),*/
      maxSpeed(maxSpeed),
      asteroidCount(asteroidCount),
      engine(dev()),
      randDirection(-180, 180),
      randSpeed((int) maxSpeed / 4, (int) maxSpeed),
      randX(0, (int) screen_width),
      randY(0, (int) screen_height),
      randRotationSpeed(-90, 90)
 {
  CreateAsteroids(asteroidCount);
  centerPoint = {(float) screen_width / 2, (float) screen_height / 2};
  pShip = new Ship(centerPoint, -90.0, 0, 0, 1, 0.5, 4.0);
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
    controller.HandleInput(running, pShip, fire);
    Update(secs_per_frame, fire);
    renderer.Render(objects);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(level, lives, score, frame_count);
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
  // Fire bullets
  if (pShip && fire) {
    auto *pBullet = new Bullet(pShip->getPosition(), pShip->getRotationAngle(), 3 * maxSpeed);
    objects.emplace_back(pBullet);
  }

  // Move objects
  for (auto &pObject: objects) {
    if (!pObject->isVisible())
       pObject->wrapAround();
    pObject->move(secs_per_frame);
  }

  // Bullets hitting asteroids
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
            switch (pAsteroid->getGeneration()){
              case 1: score += 20; break;
              case 2: score += 50; break;
              case 3: score += 100; break;
            }
            if (pAsteroid->getGeneration() == 3) {
              pAsteroid->setDestroyed(true);
            }
            else {
              pAsteroid->setGeneration(pAsteroid->getGeneration() + 1);
              pAsteroid->resize(0.75);
/*
              std::cout << std::fixed << std::setprecision(1) << std::setfill(' ')
                        << std::setw(6) << pBullet->getDirectionAngle() << " "
                        << std::setw(6) << pAsteroid->getDirectionAngle() << " ";
*/
              auto angleChange = (pBullet->getDirectionAngle() - pAsteroid->getDirectionAngle()) / 6;
              auto baseAngle = pAsteroid->getDirectionAngle();
              pAsteroid->setDirectionAngle(baseAngle + angleChange);
              pAsteroid->setSpeed((float) randSpeed(engine));
              //pAsteroid->setSpeed();
              auto pNewAsteroid = new Asteroid(*pAsteroid);
              pNewAsteroid->setDirectionAngle(baseAngle - angleChange);
              pNewAsteroid->setSpeed((float) randSpeed(engine));
              //pNewAsteroid->setSpeed();
              objects.emplace_back(pNewAsteroid);
              asteroids.emplace_back(pNewAsteroid);
/*
              std::cout << std::setw(6) << angleChange << " "
                        << std::setw(6) << pAsteroid->getDirectionAngle() << " "
                        << std::setw(6) << pNewAsteroid->getDirectionAngle() << " " << std::endl;
*/
            }
            for (int i = 0; i < 3; i++)
              objects.emplace_back(new Dust(pAsteroid->getPosition(), (float) randDirection(engine), maxSpeed));
            break;
          }
        }
      }
    }
  }

  // Asteroids hitting the ship
  if (pShip && !pShip->getGhost())
      for (auto &pObject: objects) {
        auto pAsteroid = dynamic_cast<Asteroid*>(pObject.get());
        if (pAsteroid != nullptr)
        if (pShip->collision((*pAsteroid))) {
          if (lives > 0) {
            lives--;
            if (lives > 0) {
              pShip->setGhost(true);
              pShip->setPosition(centerPoint);
            } else
              pShip->setDestroyed(true);

            for (int i = 0; i < 60; i++)
              objects.emplace_back(new Dust(pAsteroid->getPosition(), (float) randDirection(engine), maxSpeed));
          }
          break;
        }
      }
  //if (pShip)
  //  std::cout << "Ship Ghost Mode: " << pShip->getGhost() << std::endl;


  // Remove destroyed objects
  for (auto it = objects.begin(); it != objects.end();) {
    auto &pObject = *it;
    if (pObject->getDestroyed()) {
      auto ptrShip = dynamic_cast<Ship*>(pObject.get());
      auto ptrAsteroid = dynamic_cast<Asteroid*>(pObject.get());

      if (ptrShip != nullptr)
        pShip = nullptr;
      else if (ptrAsteroid != nullptr) {
        auto it2 = std::find(asteroids.begin(), asteroids.end(), ptrAsteroid);
        if (it2 != asteroids.end())
          asteroids.erase(it2);
      }
      it = objects.erase(it);
    }
    else
      ++it;
  }

  if (asteroids.empty()) {
    level++;
    asteroidCount++;
    CreateAsteroids(asteroidCount);
    pShip->setGhost(true);
  }
}

void Game::CreateAsteroids(int count) {
  for (int i = 0; i < count; i++) {
    Point position = {(float) randX(engine), (float) randY(engine)};
    auto pAsteroid = new Asteroid(position, (float) randDirection(engine), (float) randSpeed(engine),
                                  (float) randRotationSpeed(engine), engine);
    objects.emplace_back(pAsteroid);
    asteroids.emplace_back(pAsteroid);
  }

}
