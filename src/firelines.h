#include "main.h"

#ifndef FIRELINES_H
#define FIRELINES_H

class Firelines
{
  public:
    Firelines() {}
    Firelines(float x, float y, float length, float rotation);
    glm::vec3 position;
    float rotation;
    float length;
    bool exist;
    void draw(glm::mat4 VP);
    bounding_box_t box;

  private:
    VAO *object1;
    VAO *object2;
};

#endif