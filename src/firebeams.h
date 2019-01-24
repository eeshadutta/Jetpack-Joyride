#include "main.h"

#ifndef FIREBEAMS_H
#define FIREBEAMS_H

class Firebeams
{
  public:
    Firebeams() {}
    Firebeams(float x, float y, float length);
    glm::vec3 position;
    float rotation;
    int direction;
    void draw(glm::mat4 VP);
    void tick();
    bounding_box_t box;

  private:
    VAO *object1;
    VAO *object2;
};

#endif