#include "main.h"

#ifndef DRAGON_H
#define DRAGON_H

class Dragon
{
  public:
    Dragon() {}
    Dragon(float x, float y);
    glm::vec3 position;
    float rotation;
    float speed_x, speed_y;
    void draw(glm::mat4);
    void tick();

  private:
    VAO *object1;
    VAO *object2;
    VAO *object3;
    VAO *object4;
    VAO *object5;
};

#endif