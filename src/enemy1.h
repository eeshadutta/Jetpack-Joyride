#include "main.h"

#ifndef ENEMY1_H
#define ENEMY1_H

class Enemy1
{
  public:
    Enemy1() {}
    Enemy1(float x, float y, float length, float rotation);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    bounding_box_t box;

  private:
    VAO *object;
};

#endif