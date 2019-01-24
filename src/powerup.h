#include "main.h"

#ifndef POWERUP_H
#define POWERUP_H

class Powerup
{
  public:
    Powerup() {}
    Powerup(float x, float y);
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
};

#endif