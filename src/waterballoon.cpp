#include "waterballoon.h"
#include "main.h"
#define pi 3.14159265358979323846

WaterBalloon::WaterBalloon(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->move = false;

    int n = 100;
    float r = 0.3;
    float vertex_buffer_data[9 * n];
    float theta = (2 * pi) / n;
    for (int i = 0; i < 9 * n; i += 9)
    {
        vertex_buffer_data[i] = r * cos(theta);
        vertex_buffer_data[i + 1] = r * sin(theta);
        vertex_buffer_data[i + 2] = 0.0f;
        vertex_buffer_data[i + 3] = r * cos(theta + (2 * pi) / n);
        vertex_buffer_data[i + 4] = r * sin(theta + (2 * pi) / n);
        vertex_buffer_data[i + 5] = 0.0f;
        for (int j = 6; j < 9; j++)
            vertex_buffer_data[i + j] = 0.0f;
        theta += (2 * pi) / n;
    }

    this->object = create3DObject(GL_TRIANGLES, 3 * n, vertex_buffer_data, COLOR_BLUE, GL_FILL);
}

void WaterBalloon::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void WaterBalloon::tick(float speed_x, float speed_y)
{
    this->position.x += speed_x;
    this->position.y += speed_y;
}