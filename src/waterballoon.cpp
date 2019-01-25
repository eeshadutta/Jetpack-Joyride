#include "waterballoon.h"
#include "main.h"
#define pi 3.14159265358979323846

WaterBalloon::WaterBalloon(float x, float y, float radius, color_t color_out, color_t color_in)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->move = false;
    // this->speed_x = 0.1;
    // this->speed_y = 0.4;
    // this->radius = 0.3;
    this->exist = true;

    this->box.x = x;
    this->box.y = y;
    this->box.width = 2 * this->radius;
    this->box.height = 2 * this->radius;

    int n = 100;
    float r = radius;
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
    this->object1 = create3DObject(GL_TRIANGLES, 3 * n, vertex_buffer_data, color_out, GL_FILL);
    
    n = 100;
    r = radius - 0.05;
    theta = (2 * pi) / n;
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
    this->object2 = create3DObject(GL_TRIANGLES, 3 * n, vertex_buffer_data, color_in, GL_FILL);
}

void WaterBalloon::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object1);
    draw3DObject(this->object2);
}

void WaterBalloon::tick()
{
    this->position.x += this->speed_x;
    this->position.y += this->speed_y;
    this->box.x = this->position.x;
    this->box.y = this->position.y;
}