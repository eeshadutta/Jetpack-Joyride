#include "main.h"

#ifndef WATERBALLOON_H
#define WATERBALLOON_H

class WaterBalloon
{
  public:
    WaterBalloon() {}
    WaterBalloon(float x, float y);
    glm::vec3 position;
    float rotation;
    bool move;
    void draw(glm::mat4 VP);
    void tick(float speed_x, float speed_y);

  private:
    VAO *object;
};

#endif