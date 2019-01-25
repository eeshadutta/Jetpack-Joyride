#include "main.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player
{
public:
  Player() {}
  Player(float x, float y);
  glm::vec3 position;
  float rotation;
  float magnet_influence_x, magnet_influence_y;
  void draw(glm::mat4 VP);
  void set_position(float x, float y);
  void tick(float speed_x, float speed_y);
  bounding_box_t box;

private:
  VAO *object1;
  VAO *object2;
  VAO *object3;
  VAO *object4;
  VAO *object5;
  VAO *object6;
  VAO *object7;
  VAO *object8;
};

#endif