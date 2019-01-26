#include "main.h"

#ifndef RING_H
#define RING_H

class Ring
{
  public:
    Ring() {}
    Ring(float x, float y);
    glm::vec3 position;
    float rotation;
    float radius;
    void draw(glm::mat4 VP);

  private:
    VAO *object1;
    VAO *object2;
};

#endif
