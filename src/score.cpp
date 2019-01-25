#include "main.h"
#include "score.h"

Score::Score(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->exist1 = false;
    this->exist2 = false;
    this->exist3 = false;
    this->exist4 = false;
    this->exist5 = false;
    this->exist6 = false;
    this->exist7 = false;

    static const GLfloat vertex1[] = {
        -0.2, 0.7, 0.0,
        0.2, 0.7, 0.0,
        -0.2, 0.5, 0.0,
        -0.2, 0.5, 0.0,
        0.2, 0.7, 0.0,
        0.2, 0.5, 0.0,
    };
    static const GLfloat vertex2[] = {
        0.3, 0.5, 0.0,
        0.5, 0.5, 0.0,
        0.3, 0.1, 0.0,
        0.3, 0.1, 0.0,
        0.5, 0.5, 0.0, 
        0.5, 0.1, 0.0,
    };
    static const GLfloat vertex3[] = {
        0.3, -0.1, 0.0,
        0.5, -0.1, 0.0,
        0.3, -0.5, 0.0,
        0.3, -0.5, 0.0,
        0.5, -0.1, 0.0,
        0.5, -0.5, 0.0,
    };
    static const GLfloat vertex4[] = {
        -0.2, -0.5, 0.0,
        0.2, -0.5, 0.0,
        -0.2, -0.7, 0.0,
        0.2, -0.5, 0.0,
        -0.2, -0.7, 0.0,
        0.2, -0.7, 0.0,
    };
    static const GLfloat vertex5[] = {
        -0.5, -0.1, 0.0,
        -0.3, -0.1, 0.0,
        -0.5, -0.5, 0.0,
        -0.3, -0.1, 0.0,
        -0.5, -0.5, 0.0,
        -0.3, -0.5, 0.0,
    };
    static const GLfloat vertex6[] = {
        -0.5, 0.5, 0.0,
        -0.3, 0.5, 0.0,
        -0.5, 0.1, 0.0,
        -0.3, 0.5, 0.0,
        -0.5, 0.1, 0.0,
        -0.3, 0.1, 0.0,
    };

    static const GLfloat vertex7[] = {
        -0.2, 0.1, 0.0,
        0.2, 0.1, 0.0,
        -0.2, -0.1, 0.0,
        0.2, 0.1, 0.0,
        -0.2, -0.1, 0.0,
        0.2, -0.1, 0.0,
    };

    this->object1 = create3DObject(GL_TRIANGLES, 3 * 2, vertex1, COLOR_BLACK, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, 3 * 2, vertex2, COLOR_BLACK, GL_FILL);
    this->object3 = create3DObject(GL_TRIANGLES, 3 * 2, vertex3, COLOR_BLACK, GL_FILL);
    this->object4 = create3DObject(GL_TRIANGLES, 3 * 2, vertex4, COLOR_BLACK, GL_FILL);
    this->object5 = create3DObject(GL_TRIANGLES, 3 * 2, vertex5, COLOR_BLACK, GL_FILL);
    this->object6 = create3DObject(GL_TRIANGLES, 3 * 2, vertex6, COLOR_BLACK, GL_FILL);
    this->object7 = create3DObject(GL_TRIANGLES, 3 * 2, vertex7, COLOR_BLACK, GL_FILL);
}

void Score::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    
    if (this->exist1)
        draw3DObject(this->object1);
    if (this->exist2)
        draw3DObject(this->object2);
    if (this->exist3)
        draw3DObject(this->object3);
    if (this->exist4)
        draw3DObject(this->object4);
    if (this->exist5)
        draw3DObject(this->object5);
    if (this->exist6)
        draw3DObject(this->object6);
    if (this->exist7)
        draw3DObject(this->object7);
}