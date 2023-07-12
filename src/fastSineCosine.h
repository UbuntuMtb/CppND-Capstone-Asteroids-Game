//
// Created by rene on 7/07/23.
//
#ifndef FASTSINECOSINE_H
#define FASTSINECOSINE_H

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



#endif //FASTSINECOSINE_H
