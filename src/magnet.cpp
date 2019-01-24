#include "magnet.h"
#include "main.h"
#define pi 3.14159265358979323846

Magnet::Magnet(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->exist = false;

    int n = 100;
    float r1 = 1.0;
    float g_vertex_buffer_data[9 * n / 2];
    float theta = 0;
    for (int i = 0; i < 9 * n / 2; i += 9)
    {
        g_vertex_buffer_data[i] = r1 * cos(theta);
        g_vertex_buffer_data[i + 1] = r1 * sin(theta);
        g_vertex_buffer_data[i + 2] = 0.0f;
        g_vertex_buffer_data[i + 3] = r1 * cos(theta + (2 * pi) / n);
        g_vertex_buffer_data[i + 4] = r1 * sin(theta + (2 * pi) / n);
        g_vertex_buffer_data[i + 5] = 0.0f;
        for (int j = 6; j < 9; j++)
            g_vertex_buffer_data[i + j] = 0.0f;
        theta += (2 * pi) / n;
    }
    this->object1 = create3DObject(GL_TRIANGLES, 3 * n / 2, g_vertex_buffer_data, COLOR_MAROON, GL_FILL);
    
    n = 100;
    float r2 = 0.5;
    g_vertex_buffer_data[9 * n / 2];
    theta = 0;
    for (int i = 0; i < 9 * n / 2; i += 9)
    {
        g_vertex_buffer_data[i] = r2 * cos(theta);
        g_vertex_buffer_data[i + 1] = r2 * sin(theta);
        g_vertex_buffer_data[i + 2] = 0.0f;
        g_vertex_buffer_data[i + 3] = r2 * cos(theta + (2 * pi) / n);
        g_vertex_buffer_data[i + 4] = r2 * sin(theta + (2 * pi) / n);
        g_vertex_buffer_data[i + 5] = 0.0f;
        for (int j = 6; j < 9; j++)
            g_vertex_buffer_data[i + j] = 0.0f;
        theta += (2 * pi) / n;
    }
    this->object2 = create3DObject(GL_TRIANGLES, 3 * n / 2, g_vertex_buffer_data, COLOR_BACKGROUND, GL_FILL);
    
    static const GLfloat vertex_buffer_data[] = {
        r2, 0.0, 0.0,
        r2, -r1, 0.0,
        r1, 0.0, 0.0,
        r1, 0.0, 0.0,
        r2, -r1, 0.0,
        r1, -r1, 0.0,
        -r2, 0.0, 0.0,
        -r2, -r1, 0.0,
        -r1, 0.0, 0.0,
        -r1, 0.0, 0.0,
        -r2, -r1, 0.0,
        -r1, -r1, 0.0,
    };
    this->object3 = create3DObject(GL_TRIANGLES, 3 * 4, vertex_buffer_data, COLOR_BLACK, GL_FILL);
}

void Magnet::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object1);
    draw3DObject(this->object2);
    draw3DObject(this->object3);
}