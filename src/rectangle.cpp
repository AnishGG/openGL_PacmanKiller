#include "rectangle.h"
#include "main.h"

/* Diagnol Points need to be p2 and p3 */
Rectangle::Rectangle(Point p1, Point p2, Point p3, Point p4, color_t color) {
    this->position = glm::vec3(0, 0, 0);
    this->rotation = 0;
    this->theta = 0;
    this->max_point = std::max(p2.y, p3.y);

    GLfloat vertex_buffer_data[] = {
        p1.x, p1.y, 0,
        p2.x, p2.y, 0,
        p3.x, p3.y, 0,

        p2.x, p2.y, 0,
        p3.x, p3.y, 0,
        p4.x, p4.y, 0
    };

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}

void Rectangle::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Rectangle::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Rectangle::tick() {
    this->position.x += this->speed;
}

void Rectangle::set_speed(float speed){
    this->speed = speed;
}

void Rectangle::set_theta(float theta){
    this->theta = theta;
}
