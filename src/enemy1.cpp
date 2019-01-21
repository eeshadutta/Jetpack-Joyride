#include "enemy1.h"
#include "main.h"
#define pi 3.14159265358979323846

Enemy1::Enemy1(float x, float y, float length, float rotation)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = rotation;

    int n = 100;
    float r = 0.3;
    float vertex_buffer_data[18 * (n + 1)];
    float theta = (2 * pi) / n;
    for (int i = 0; i < 9 * n; i += 9)
    {
        vertex_buffer_data[i] = length / 2 + r * cos(theta);
        vertex_buffer_data[i + 1] = r * sin(theta);
        vertex_buffer_data[i + 2] = 0.0f;
        vertex_buffer_data[i + 3] = length / 2 + r * cos(theta + (2 * pi) / n);
        vertex_buffer_data[i + 4] = r * sin(theta + (2 * pi) / n);
        vertex_buffer_data[i + 5] = 0.0f;
        vertex_buffer_data[i + 6] = length / 2 + 0.0f;
        vertex_buffer_data[i + 7] = 0.0f;
        vertex_buffer_data[i + 8] = 0.0f;
        theta += (2 * pi) / n;
    }
    for (int i = 9 * n; i < 18 * n; i += 9)
    {
        vertex_buffer_data[i] = -length / 2 + r * cos(theta);
        vertex_buffer_data[i + 1] = r * sin(theta);
        vertex_buffer_data[i + 2] = 0.0f;
        vertex_buffer_data[i + 3] = -length / 2 + r * cos(theta + (2 * pi) / n);
        vertex_buffer_data[i + 4] = r * sin(theta + (2 * pi) / n);
        vertex_buffer_data[i + 5] = 0.0f;
        vertex_buffer_data[i + 6] = -length / 2 + 0.0f;
        vertex_buffer_data[i + 7] = 0.0f;
        vertex_buffer_data[i + 8] = 0.0f;
        theta += (2 * pi) / n;
    }
    vertex_buffer_data[18 * n] = -length / 2;
    vertex_buffer_data[18 * n + 1] = -r / 2;
    vertex_buffer_data[18 * n + 2] = 0.0f;
    vertex_buffer_data[18 * n + 3] = -length / 2;
    vertex_buffer_data[18 * n + 4] = r / 2;
    vertex_buffer_data[18 * n + 5] = 0.0f;
    vertex_buffer_data[18 * n + 6] = length / 2;
    vertex_buffer_data[18 * n + 7] = -r / 2;
    vertex_buffer_data[18 * n + 8] = 0.0f;
    vertex_buffer_data[18 * n + 9] = -length / 2;
    vertex_buffer_data[18 * n + 10] = r / 2;
    vertex_buffer_data[18 * n + 11] = 0.0f;
    vertex_buffer_data[18 * n + 12] = length / 2;
    vertex_buffer_data[18 * n + 13] = -r / 2;
    vertex_buffer_data[18 * n + 14] = 0.0f;
    vertex_buffer_data[18 * n + 15] = length / 2;
    vertex_buffer_data[18 * n + 16] = r / 2;
    vertex_buffer_data[18 * n + 17] = 0.0f;

    box.x = x;
    box.y = y;
    box.width = length + 2 * r;
    box.height = r;

    this->object = create3DObject(GL_TRIANGLES, 6 * (n + 1), vertex_buffer_data, COLOR_YELLOW, GL_FILL);
}

void Enemy1::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}