#include "wall.h"
#include "main.h"

Wall::Wall(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    static const GLfloat vertex_buffer_data[] = {
        0.0f, 0.0f, 0.0f,
        10.0f, 0.0f, 0.0f,
        0.0f, 15.0f, 0.0f,
        0.0f, 15.0f, 0.0f,
        10.0f, 0.0f, 0.0f,
        10.0f, 15.0f, 0.0f,

        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        9.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        9.0f, -1.0f, 0.0f,
        9.0f, 0.0f, 0.0f,
        9.0f, -1.0f, 0.0f,
        9.0f, 0.0f, 0.0f, 
        10.0f, 0.0f, 0.0f,

        0.0f, 15.0f, 0.0f,
        1.0f, 15.0f, 0.0f,
        1.0f, 16.0f, 0.0f,
        1.0f, 15.0f, 0.0f,
        1.0f, 16.0f, 0.0f,
        9.0f, 15.0f, 0.0f,
        1.0f, 16.0f, 0.0f,
        9.0f, 15.0f, 0.0f,
        9.0f, 16.0f, 0.0f,
        9.0f, 16.0f, 0.0f,
        9.0f, 15.0f, 0.0f,
        10.0f, 15.0f, 0.0f,

        15.0f, 0.0f, 0.0f,
        20.0f, 0.0f, 0.0f,
        15.0f, 15.0f, 0.0f,
        15.0f, 15.0f, 0.0f,
        20.0f, 0.0f, 0.0f,
        20.0f, 15.0f, 0.0f,

        15.0f, 0.0f, 0.0f,
        16.0f, 0.0f, 0.0f,
        16.0f, -1.0f, 0.0f,
        16.0f, 0.0f, 0.0f,
        16.0f, -1.0f, 0.0f,
        19.0f, -1.0f, 0.0f,
        19.0f, -1.0f, 0.0f,        
        16.0f, 0.0f, 0.0f,
        19.0f, 0.0f, 0.0f,
        19.0f, 0.0f, 0.0f,
        19.0f, -1.0f, 0.0f,        
        20.0f, 0.0f, 0.0f,

        15.0f, 15.0f, 0.0f,
        16.0f, 15.0f, 0.0f,
        16.0f, 16.0f, 0.0f,
        16.0f, 16.0f, 0.0f,
        16.0f, 15.0f, 0.0f,
        19.0f, 15.0f, 0.0f,
        19.0f, 15.0f, 0.0f,
        16.0f, 16.0f, 0.0f,
        19.0f, 16.0f, 0.0f,
        19.0f, 16.0f, 0.0f,
        19.0f, 15.0f, 0.0f,
        20.0f, 15.0f, 0.0f,
    };
    this->object = create3DObject(GL_TRIANGLES, 20*3, vertex_buffer_data, COLOR_LIGHT_GREY, GL_FILL);
}

void Wall::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); 
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}