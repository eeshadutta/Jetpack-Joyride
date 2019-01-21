#include "coin.h"
#include "main.h"
#define pi 3.14159265358979323846

Coin::Coin(float x, float y, int type)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->exist = true;
    this->type = type;

    box.x = x;
    box.y = y;
    box.width = 0.6;
    box.height = 0.6;

    int n = 100;
    float r = 0.3;
    float g_vertex_buffer_data[9 * n];
    float theta = (2 * pi) / n;
    for (int i = 0; i < 9 * n; i += 9)
    {
        g_vertex_buffer_data[i] = r * cos(theta);
        g_vertex_buffer_data[i + 1] = r * sin(theta);
        g_vertex_buffer_data[i + 2] = 0.0f;
        g_vertex_buffer_data[i + 3] = r * cos(theta + (2 * pi) / n);
        g_vertex_buffer_data[i + 4] = r * sin(theta + (2 * pi) / n);
        g_vertex_buffer_data[i + 5] = 0.0f;
        for (int j = 6; j < 9; j++)
            g_vertex_buffer_data[i + j] = 0.0f;
        theta += (2 * pi) / n;
    }
    
    if (this->type == 1)
        this->object = create3DObject(GL_TRIANGLES, 3 * n, g_vertex_buffer_data, COLOR_COIN1, GL_FILL);
    else if (this->type == 2)
        this->object = create3DObject(GL_TRIANGLES, 3 * n, g_vertex_buffer_data, COLOR_COIN2, GL_FILL);
    else if (this->type == 3)
        this->object = create3DObject(GL_TRIANGLES, 3 * n, g_vertex_buffer_data, COLOR_COIN3, GL_FILL);
    else if (this->type == 4)
        this->object = create3DObject(GL_TRIANGLES, 3 * n, g_vertex_buffer_data, COLOR_COIN4, GL_FILL);
}

void Coin::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}