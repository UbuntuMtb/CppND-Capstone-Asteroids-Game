//
// Created by rene on 7/07/23.
//
#ifndef FASTSINECOSINE_H
#define FASTSINECOSINE_H

class Sine {
public:
  Sine();
  float operator()(int angle) const;
private:
  float values[360];
};

class Cosine {
public:
  Cosine();
  float operator()(int angle) const;
private:
  float values[360];
};



#endif //FASTSINECOSINE_H
