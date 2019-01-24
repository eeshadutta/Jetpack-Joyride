#include "boomerang.h"
#include "main.h"

Boomerang::Boomerang(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->direction = -1;
    this->exist = false;

    this->box.x = x;
    this->box.y = y;
    this->box.width = 4.8;
    this->box.height = 2.4;

    static const GLfloat vertex_data_1[] = {
        0.0, 0.0, 0.0,
        -0.3, 0.3, 0.0,
        -0.3, -0.3, 0.0,
        -0.6, 0.0, 0.0,
        -0.9, 0.3, 0.0,
        -0.9, -0.3, 0.0,
        -1.2, 1.2, 0.0,
        -1.2, -1.2, 0.0,
        -2.4, 0.0, 0.0,
        0.6, 0.6, 0.0,
        1.2, 0.9, 0.0,
        1.2, 0.6, 0.0,
        0.6, -0.6, 0.0,
        1.2, -0.9, 0.0,
        1.2, -0.6, 0.0,
    };
    this->object1 = create3DObject(GL_TRIANGLES, 5 * 3, vertex_data_1, COLOR_DARK_GREEN, GL_FILL);
    static const GLfloat vertex_data_2[] = {
        -1.2, 0.6, 0.0,
        1.2, 0.6, 0.0,
        -1.2, -0.6, 0.0,
        -1.2, -0.6, 0.0,
        1.2, 0.6, 0.0,
        1.2, -0.6, 0.0,
    };
    this->object2 = create3DObject(GL_TRIANGLES, 2 * 3, vertex_data_2, COLOR_YELLOW, GL_FILL);
    static const GLfloat vertex_data_3[] = {
        1.2, 0.6, 0.0,
        1.2, 0.2, 0.0,
        2.4, 0.2, 0.0,
        1.2, 0.2, 0.0,
        1.2, -0.2, 0.0,
        2.4, -0.2, 0.0,
        1.2, -0.2, 0.0,
        1.2, -0.6, 0.0,
        2.4, -0.6, 0.0,
    };
    this->object3 = create3DObject(GL_TRIANGLES, 3 * 3, vertex_data_3, COLOR_ORANGE, GL_FILL);
}

void Boomerang::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object2);
    draw3DObject(this->object1);
    draw3DObject(this->object3);
}

void Boomerang::tick(float speed_x, float speed_y)
{
    this->position.x += speed_x;
    this->position.y += speed_y;
    this->box.x = this->position.x;
    this->box.y = this->position.y;
}

void Boomerang::rotate(int angle)
{
    this->rotation += angle;
    if (this->rotation >= 180 && this->direction == -2)
    {
        this->rotation = 180;
    }
    if (this->rotation >= 360 && this->direction == 2)
    {
        this->rotation = 0;
    }
}