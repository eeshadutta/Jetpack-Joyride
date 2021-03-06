#include "main.h"

#ifndef BOOMERANG_H
#define BOOMERANG_H

class Boomerang
{
  public:
    Boomerang() {}
    Boomerang(float x, float y);
    glm::vec3 position;
    float rotation;
    bool exist;
    void draw(glm::mat4 VP);
    void tick(float speed_x, float speed_y);
    void rotate(int angle);
    int direction;
    bounding_box_t box;

  private:
    VAO *object1;
    VAO *object2;
    VAO *object3;
};

#endif