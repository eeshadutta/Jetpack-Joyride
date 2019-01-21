#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick_left();
    void tick_right();
    void tick_down();
    double speed_x;
    double speed_y;
    bounding_box_t box;
private:
    VAO *object;
};

#endif // BALL_H
