#include "main.h"

#ifndef COIN_H
#define COIN_H

class Coin
{
  public:
    Coin() {}
    Coin(float x, float y);
    glm::vec3 position;
    float rotation;
    bool exist;
    void draw(glm::mat4 VP);
    bounding_box_t box;

  private:
    VAO *object;
};

#endif