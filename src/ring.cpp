#include "ring.h"
#include "main.h"
#define pi 3.14159265358979323846

Ring::Ring(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;

    int n = 100;
    float r = 3.0;
    float vertex1[9 * n / 2];
    float theta = 0;
    for (int i = 0; i < 9 * n / 2; i += 9)
    {
        vertex1[i] = r * cos(theta);
        vertex1[i + 1] = r * sin(theta);
        vertex1[i + 2] = 0.0f;
        vertex1[i + 3] = r * cos(theta + (2 * pi) / n);
        vertex1[i + 4] = r * sin(theta + (2 * pi) / n);
        vertex1[i + 5] = 0.0f;
        for (int j = 6; j < 9; j++)
            vertex1[i + j] = 0.0f;
        theta += (2 * pi) / n;
    }
    this->object1 = create3DObject(GL_TRIANGLES, 3 * n / 2, vertex1, COLOR_MAROON, GL_FILL);
    
    n = 100;
    r = 2.5;
    float vertex2[9 * n / 2];
    theta = 0;
    for (int i = 0; i < 9 * n / 2; i += 9)
    {
        vertex2[i] = r * cos(theta);
        vertex2[i + 1] = r * sin(theta);
        vertex2[i + 2] = 0.0f;
        vertex2[i + 3] = r * cos(theta + (2 * pi) / n);
        vertex2[i + 4] = r * sin(theta + (2 * pi) / n);
        vertex2[i + 5] = 0.0f;
        for (int j = 6; j < 9; j++)
            vertex2[i + j] = 0.0f;
        theta += (2 * pi) / n;
    }
    this->object1 = create3DObject(GL_TRIANGLES, 3 * n / 2, vertex2, COLOR_MAROON, GL_FILL);

}

void Ring::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object1);
}
