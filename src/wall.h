#include "main.h"

#ifndef WALL_H
#define WALL_H

class Wall
{
  public:
    Wall() {}
    Wall(float x, float y);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);

  private:
    VAO *object;
};

#endif