#include "main.h"

#ifndef PLATFORM_H
#define PLATFORM_H

class Platform
{
public:
  Platform() {}
  Platform(float x, float y);
  glm::vec3 position;
  float rotation;
  void draw(glm::mat4 VP);

private:
  VAO *object;
};

#endif
