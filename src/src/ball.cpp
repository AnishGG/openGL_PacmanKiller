#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, color_t color, double Radius) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->Radius   = Radius;
    this->isSlabAttached = 0;
    this->speed_x = 0.01;

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
    this->position.x += this->speed_x;
    this->position.y += this->speed_y;
    if(this->position.y <= -1.8){
        jumped = 0;
        speed_y = 0;
        this->position.y = -1.8;
    }
    if(jumped){
        this->deaccelerate();
    }
}

void Ball::jump() {
    speed_y = 0.05;
    jumped = 1;
}

void Ball::deaccelerate() {
    if(speed_y > -0.2){
        speed_y -= 0.01;
    }
}

bounding_box_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}

void Ball::attach_slab(Point* points, float theta, float length, float thickness){
    float new_x1 = this->position.x + (this->Radius + thickness) * cos(theta * PI / 180.0);
    float new_y1 = this->position.y + (this->Radius + thickness) * sin(theta * PI / 180.0);
    float new_x2 = this->position.x + (this->Radius) * cos(theta * PI / 180.0);
    float new_y2 = this->position.y + (this->Radius) * sin(theta * PI / 180.0);

    Point p1, p2, p3, p4;
    p1.x = new_x1 + (length/2) * cos((theta + 90) * PI / 180.0);
    p1.y = new_y1 + (length/2) * sin((theta + 90) * PI / 180.0);
    // p2 and p3 will be diagnols of the slab

    p2.x = new_x1 + (length/2) * cos((theta + 270)* PI / 180.0);
    p2.y = new_y1 + (length/2) * sin((theta + 270)* PI / 180.0);

    p3.x = new_x2 + (length/2) * cos((theta + 90) * PI / 180.0);
    p3.y = new_y2 + (length/2) * sin((theta + 90) * PI / 180.0);
    // p1 and p4 will be diagnol points of the slab
    p4.x = new_x2 + (length/2) * cos((theta + 270)* PI / 180.0);
    p4.y = new_y2 + (length/2) * sin((theta + 270)* PI / 180.0);
    points[0] = p1;points[1] = p2;points[2] = p3;points[3] = p4;
    this->isSlabAttached = 1;
}
