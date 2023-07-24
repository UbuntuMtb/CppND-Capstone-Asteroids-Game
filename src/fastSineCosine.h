//
// Created by rene on 7/07/23.
//
#ifndef FAST_SINE_COSINE_H
#define FAST_SINE_COSINE_H

class Sine {
public:
  Sine();
  float operator()(float angle) const;
private:
  float values[360];
};

class Cosine {
public:
  Cosine();
  float operator()(float angle) const;
private:
  float values[360];
};

#endif //FAST_SINE_COSINE_H
