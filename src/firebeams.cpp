#include "firebeams.h"
#include "main.h"
#define pi 3.14159265358979323846

Firebeams::Firebeams(float x, float y, float length)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->direction = 1;

    this->box.x = x;
    this->box.y = y;
    this->box.width = length + 2.0;
    this->box.height = 2.0;

    int n = 100;
    float r = 1.0;
    float vertex_buffer_data[9 * n + 36];
    float theta = -pi / 2;
    for (int i = 0; i < 9 * n / 2; i += 9)
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
    theta = pi / 2;
    for (int i = 9 * n / 2; i < 9 * n; i += 9)
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
    vertex_buffer_data[9 * n] = -length / 2;
    vertex_buffer_data[9 * n + 1] = 0.3;
    vertex_buffer_data[9 * n + 2] = 0.0f;
    vertex_buffer_data[9 * n + 3] = -length / 2;
    vertex_buffer_data[9 * n + 4] = 0.6;
    vertex_buffer_data[9 * n + 5] = 0.0f;
    vertex_buffer_data[9 * n + 6] = length / 2;
    vertex_buffer_data[9 * n + 7] = 0.3;
    vertex_buffer_data[9 * n + 8] = 0.0f;
    vertex_buffer_data[9 * n + 9] = -length / 2;
    vertex_buffer_data[9 * n + 10] = 0.6;
    vertex_buffer_data[9 * n + 11] = 0.0f;
    vertex_buffer_data[9 * n + 12] = length / 2;
    vertex_buffer_data[9 * n + 13] = 0.3;
    vertex_buffer_data[9 * n + 14] = 0.0f;
    vertex_buffer_data[9 * n + 15] = length / 2;
    vertex_buffer_data[9 * n + 16] = 0.6;
    vertex_buffer_data[9 * n + 17] = 0.0f;
    vertex_buffer_data[9 * n + 18] = -length / 2;
    vertex_buffer_data[9 * n + 19] = -0.3;
    vertex_buffer_data[9 * n + 20] = -0.0f;
    vertex_buffer_data[9 * n + 21] = -length / 2;
    vertex_buffer_data[9 * n + 22] = -0.6;
    vertex_buffer_data[9 * n + 23] = 0.0f;
    vertex_buffer_data[9 * n + 24] = length / 2;
    vertex_buffer_data[9 * n + 25] = -0.3;
    vertex_buffer_data[9 * n + 26] = 0.0f;
    vertex_buffer_data[9 * n + 27] = -length / 2;
    vertex_buffer_data[9 * n + 28] = -0.6;
    vertex_buffer_data[9 * n + 29] = 0.0f;
    vertex_buffer_data[9 * n + 30] = length / 2;
    vertex_buffer_data[9 * n + 31] = -0.3;
    vertex_buffer_data[9 * n + 32] = 0.0f;
    vertex_buffer_data[9 * n + 33] = length / 2;
    vertex_buffer_data[9 * n + 34] = -0.6;
    vertex_buffer_data[9 * n + 35] = 0.0f;
    this->object1 = create3DObject(GL_TRIANGLES, 3 * n, vertex_buffer_data, COLOR_MAROON, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, 3 * 4, vertex_buffer_data + 9 * n, COLOR_ORANGE, GL_FILL);
}

void Firebeams::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object2);
    draw3DObject(this->object1);
}

void Firebeams::tick()
{
    if (this->direction == 1)
        this->position.y += 0.1;
    else
        this->position.y -= 0.1;
    this->box.y = this->position.y;
}