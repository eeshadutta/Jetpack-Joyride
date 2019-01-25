#include "main.h"
#include "dragon.h"
#define pi 3.14159265358979323846

Dragon::Dragon(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 45;
    this->speed_x = 0;
    this->speed_y = 0.2;

    static const GLfloat vertex1[] = {
        1.5, 2.5, 0.0,
        3.0, 2.5, 0.0,
        1.5, 1.5, 0.0,
        3.0, 2.5, 0.0,
        1.5, 1.5, 0.0,
        3.0, 1.5, 0.0,

        2.5, 1.5, 0.0,
        3.0, 1.5, 0.0,
        2.5, 1.0, 0.0,
        3.0, 1.5, 0.0,
        2.5, 1.0, 0.0,
        3.0, 1.0, 0.0,

        -3.0, 1.0, 0.0,
        3.0, 1.0, 0.0,
        -3.0, -1.0, 0.0,
        3.0, 1.0, 0.0,
        3.0, -1.0, 0.0,
        -3.0, -1.0, 0.0,

        -2.5, -1.0, 0.0,
        -3.0, -1.0, 0.0,
        -3.0, -1.5, 0.0,
        -2.5, -1.0, 0.0,
        -2.5, -1.5, 0.0,
        -3.0, -1.5, 0.0,

        -1.0, -1.5, 0.0,
        -3.0, -2.0, 0.0,
        -3.0, -1.5, 0.0,
        -3.0, -2.0, 0.0,
        -1.0, -1.5, 0.0,
        -1.0, -2.0, 0.0,
    };
    this->object1 = create3DObject(GL_TRIANGLES, 10 * 3, vertex1, COLOR_YELLOW, GL_FILL);

    static const GLfloat vertex2[] = {
        0.8, 2.5, 0.0,
        1.5, 2.5, 0.0,
        0.8, 2.0, 0.0,
        0.8, 2.0, 0.0, 
        1.5, 2.5, 0.0,
        1.5, 2.0, 0.0,

        0.8, 1.8, 0.0,
        1.5, 1.8, 0.0,
        0.8, 1.5, 0.0,
        0.8, 1.5, 0.0,
        1.5, 1.8, 0.0,
        1.5, 1.5, 0.0,

        1.0, 2.5, 0.0,
        2.0, 2.5, 0.0,
        2.4, 3.0, 0.0,
    };
    this->object2 = create3DObject(GL_TRIANGLES, 5 * 3, vertex2, COLOR_RED, GL_FILL);

    int n = 100;
    float r = 0.3;
    float vertex3[9 * n];
    float theta = 0;
    for (int i = 0; i < 9 * n; i += 9)
    {
        vertex3[i] = 1.5 + r * cos(theta);
        vertex3[i + 1] = 2.3 + r * sin(theta);
        vertex3[i + 2] = 0.0f;
        vertex3[i + 3] = 1.5 + r * cos(theta + (2 * pi) / n);
        vertex3[i + 4] = 2.3 + r * sin(theta + (2 * pi) / n);
        vertex3[i + 5] = 0.0f;
        vertex3[i + 6] = 1.5f;
        vertex3[i + 7] = 2.3f;
        vertex3[i + 8] = 0.0f;
        theta += (2 * pi) / n;
    }
    this->object3 = create3DObject(GL_TRIANGLES, 3 * n, vertex3, COLOR_BLACK, GL_FILL);
    
    static const GLfloat vertex4[] = {
        0.0, -0.5, 0.0,
        4.0, -1.0, 0.0,
        2.0, -4.0, 0.0,
    };
    this->object4 = create3DObject(GL_TRIANGLES, 3, vertex4, COLOR_RED, GL_FILL);  

    static const GLfloat vertex5[] = {
        1.0, 0.0, 0.0,
        4.0, -4.0, 0.0,
        6.0, -0.8, 0.0,
    };
    this->object5 = create3DObject(GL_TRIANGLES, 3, vertex5, COLOR_RED, GL_FILL);  

}

void Dragon::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); 
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object5);
    draw3DObject(this->object1);
    draw3DObject(this->object2);
    draw3DObject(this->object3);
    draw3DObject(this->object4);
}

void Dragon::tick()
{
    this->position.x += this->speed_x;
    this->position.y += this->speed_y;
}