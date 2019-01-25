#include "main.h"

#ifndef SCORE_H
#define SCORE_H

class Score
{
  public:
    Score() {}
    Score(float x, float y);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    bool exist1, exist2, exist3, exist4, exist5, exist6, exist7;

  private:
    VAO *object1;
    VAO *object2;
    VAO *object3;
    VAO *object4;
    VAO *object5;
    VAO *object6;
    VAO *object7;
};

#endif