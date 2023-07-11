#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "asteroid.h"

class Controller {
 public:
  void HandleInput(bool &running, Ship &ship, bool &fire) const;

 private:
};

#endif