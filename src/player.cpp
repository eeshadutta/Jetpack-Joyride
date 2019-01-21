#include "player.h"
#include "main.h"

Player::Player(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    box.x = x;
    box.y = y;
    box.width = 2.0;
    box.height = 2.0;
    static const GLfloat vertex_buffer_data[] = {
        1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
    };
    this->object = create3DObject(GL_TRIANGLES, 2 * 3, vertex_buffer_data, COLOR_GREEN, GL_FILL);
}

void Player::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); 
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Player::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Player::tick(float speed_x, float speed_y)
{
    this->position.x += speed_x;
    this->position.y += speed_y;
    box.x = this->position.x;
    box.y = this->position.y;
}