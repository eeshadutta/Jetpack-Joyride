#include "main.h"

#ifndef WATERBALLOON_H
#define WATERBALLOON_H

class WaterBalloon
{
  public:
    WaterBalloon() {}
    WaterBalloon(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    bool move;
    float speed_x, speed_y;
    float radius;
    bool exist;
    void draw(glm::mat4 VP);
    void tick();
    bounding_box_t box;

  private:
    VAO *object;
};

#endif