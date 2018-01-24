#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, color_t color, double Radius) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 0.01;
    static const double PI = 3.14159265;

    GLfloat vertex_buffer_data[10010];
    double theta = 1.0;
    int i = 0;
    while(theta < 362.0){
        vertex_buffer_data[i] = Radius * cos((theta * PI) / 180.0);
        vertex_buffer_data[i+1] = Radius * sin((theta * PI) / 180.0);
        vertex_buffer_data[i+2] = 0.0;
        vertex_buffer_data[i+3] = 0;
        vertex_buffer_data[i+4] = 0;
        vertex_buffer_data[i+5] = 0;
        theta += 1.0;
        vertex_buffer_data[i+6] = Radius * cos(theta * PI / 180.0);
        vertex_buffer_data[i+7] = Radius * sin(theta * PI / 180.0);
        vertex_buffer_data[i+8] = 0;
        i += 9;
    }

    this->object = create3DObject(GL_TRIANGLES, i/3, vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
//    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    this->position.x += this->speed;
}

void Ball::jump() {
    speed = 0.3;
}

void Ball::deaccelerate() {
    speed -= 0.01;
}

bounding_box_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}
