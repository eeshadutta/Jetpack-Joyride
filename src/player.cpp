#include "player.h"
#include "main.h"
#define pi 3.14159265358979323846

Player::Player(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->magnet_influence_x = 0;
    this->magnet_influence_y = 0;

    box.x = x;
    box.y = y;
    box.width = 2.0;
    box.height = 2.0;

    int n = 100;
    float r = 0.4;
    float vertex_buffer_data[9 * n];
    float theta = (2 * pi) / n;
    for (int i = 0; i < 9 * n; i += 9)
    {
        vertex_buffer_data[i] = 0.6 + r * cos(theta);
        vertex_buffer_data[i + 1] = 1.6 + r * sin(theta);
        vertex_buffer_data[i + 2] = 0.0f;
        vertex_buffer_data[i + 3] = 0.6 + r * cos(theta + (2 * pi) / n);
        vertex_buffer_data[i + 4] = 1.6 + r * sin(theta + (2 * pi) / n);
        vertex_buffer_data[i + 5] = 0.0f;
        vertex_buffer_data[i + 6] = 0.6f;
        vertex_buffer_data[i + 7] = 1.6f;
        vertex_buffer_data[i + 8] = 0.0f;
        theta += (2 * pi) / n;
    }
    this->object1 = create3DObject(GL_TRIANGLES, 3 * n, vertex_buffer_data, COLOR_ORANGE, GL_FILL);

    static const GLfloat vertex2[] = {
        0.0, 1.2, 0.0,
        0.0, -0.3, 0.0,
        1.0, -0.3, 0.0,
        0.0, 1.2, 0.0,
        1.0, -0.3, 0.0,
        1.0, 1.2, 0.0,
    };
    this->object2 = create3DObject(GL_TRIANGLES, 3 * 2, vertex2, COLOR_RED, GL_FILL);

    static const GLfloat vertex3[] = {
        0.0, -0.3, 0.0,
        0.5, -0.3, 0.0,
        -0.2, -2.0, 0.0,
        0.5, -0.3, 0.0,
        -0.2, -2.0, 0.0,
        0.3, -2.0, 0.0,

        0.5, -0.3, 0.0,
        0.7, -2.0, 0.0,
        1.2, -2.0, 0.0,
        0.5, -0.3, 0.0,
        1.2, -2.0, 0.0,
        1.0, -0.3, 0.0,
    };
    this->object3 = create3DObject(GL_TRIANGLES, 3 * 4, vertex3, COLOR_BLACK, GL_FILL);

    static const GLfloat vertex4[] = {
        0.5, 0.4, 0.0,
        1.2, 0.0, 0.0,
        0.6, 0.8, 0.0,
        1.2, 0.0, 0.0,
        0.6, 0.8, 0.0,
        1.3, 0.4, 0.0,

        1.2, 0.0, 0.0,
        1.0, 0.3, 0.0,
        2.0, 0.3, 0.0,
        1.0, 0.3, 0.0,
        2.0, 0.3, 0.0,
        1.8, 0.6, 0.0, 
    };
    this->object4 = create3DObject(GL_TRIANGLES, 3 * 4, vertex4, COLOR_BLACK, GL_FILL);

    static const GLfloat vertex5[] = {
        -1.0, -0.3, 0.0,
        -1.0, 1.0, 0.0,
        0.0, -0.3, 0.0,
        -1.0, 1.0, 0.0,
        0.0, -0.3, 0.0,
        0.0, 1.0, 0.0,
    };
    this->object5 = create3DObject(GL_TRIANGLES, 3 * 2, vertex5, COLOR_BLACK, GL_FILL);

    static const GLfloat vertex6[] = {
        -0.8, 1.0, 0.0,
        -0.8, 1.8, 0.0,
        -0.2, 1.0, 0.0,
        -0.8, 1.8, 0.0,
        -0.2, 1.0, 0.0,
        -0.2, 1.8, 0.0,
    };
    this->object6 = create3DObject(GL_TRIANGLES, 3 * 2, vertex6, COLOR_MAROON, GL_FILL);

    n = 100;
    r = 0.8;
    float vertex7[9 * n / 2 + 9 * n / 4];
    theta = pi / 4;
    for (int i = 0; i < 9 * n / 2 + 9 * n / 4; i += 9)
    {
        vertex7[i] = -1.0 + r * cos(theta);
        vertex7[i + 1] = 0.8 + r * sin(theta);
        vertex7[i + 2] = 0.0;
        vertex7[i + 3] = -1.0 + r * cos(theta + (2 * pi) / n);
        vertex7[i + 4] = 0.8 + r * sin(theta + (2 * pi) / n);
        vertex7[i + 5] = 0.0;
        vertex7[i + 6] = -1.0;
        vertex7[i + 7] = 0.8;
        vertex7[i + 8] = 0.0;
        theta += (2 * pi) / n;
    }
    this->object7 = create3DObject(GL_TRIANGLES, 3 * n / 2 + 3 * n / 4, vertex7, COLOR_YELLOW, GL_FILL);
    
    n = 100;
    r = 0.6;
    float vertex8[9 * n / 2 + 9 * n / 4];
    theta = pi / 4;
    for (int i = 0; i < 9 * n / 2 + 9 * n / 4; i += 9)
    {
        vertex8[i] = -1.0 + r * cos(theta);
        vertex8[i + 1] = 0.8 + r * sin(theta);
        vertex8[i + 2] = 0.0;
        vertex8[i + 3] = -1.0 + r * cos(theta + (2 * pi) / n);
        vertex8[i + 4] = 0.8 + r * sin(theta + (2 * pi) / n);
        vertex8[i + 5] = 0.0;
        vertex8[i + 6] = -1.0;
        vertex8[i + 7] = 0.8;
        vertex8[i + 8] = 0.0;
        theta += (2 * pi) / n;
    }
    this->object8 = create3DObject(GL_TRIANGLES, 3 * n / 2 + 3 * n / 4, vertex8, COLOR_BACKGROUND, GL_FILL);

}

void Player::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object7);
    draw3DObject(this->object8);
    draw3DObject(this->object5);
    draw3DObject(this->object1);
    draw3DObject(this->object2);
    draw3DObject(this->object3);
    draw3DObject(this->object4);
    draw3DObject(this->object6);
}

void Player::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Player::tick(float speed_x, float speed_y)
{
    this->position.x += speed_x + this->magnet_influence_x;
    this->position.y += speed_y + this->magnet_influence_y;
    box.x = this->position.x;
    box.y = this->position.y;
}