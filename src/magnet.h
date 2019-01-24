#include "main.h"

#ifndef MAGNET_H
#define MAGNET_H

class Magnet
{
  public:
    Magnet() {}
    Magnet(float x, float y);
    glm::vec3 position;
    float rotation;
    bool exist;
    void draw(glm::mat4 VP);

  private:
    VAO *object1;
    VAO *object2;
    VAO *object3;
};

#endif