#include "main.h"

#ifndef PIGGYBANK_H
#define PIGGYBANK_H

class Piggybank
{
  public:
    Piggybank() {}
    Piggybank(float x, float y);
    glm::vec3 position;
    float rotation;
    float speed_x, speed_y;
    bool exist;
    void draw(glm::mat4 VP);
    void tick();
    bounding_box_t box;

  private:
    VAO *object1;
    VAO *object2;
    VAO *object3;
};

#endif