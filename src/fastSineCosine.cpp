//
// Created by rene on 7/07/23.
//
#include "fastSineCosine.h"
#include <cmath>

Sine::Sine() {
  for (int i = 0; i < 360; i++)
    values[i] = (float) sin(i * 2 * M_PI / 360);
}

float Sine::operator()(float angle) const {
  if (angle >= 360 || angle <= -360)
    angle = std::fmod(angle, (float) 360);
  if (angle < 0)
    angle = 360 + angle;
  return values[(int) angle];
}

Cosine::Cosine() {
  for (int i = 0; i < 360; i++)
    values[i] = (float) cos(i * 2 * M_PI / 360);
}

float Cosine::operator()(float angle) const {
  if (angle >= 360 || angle <= -360)
    angle = std::fmod(angle, (float) 360);
  if (angle < 0)
    angle = 360 + angle;
  return values[(int) angle];
}
