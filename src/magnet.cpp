#include "magnet.h"
#include "main.h"

Magnet::Magnet(float x, float y, double length, double breadth, double angle, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->major = length*2;
    this->minor = breadth;
    this->x0 = x;
    this->y0 = y;
    this->angle = angle;

    static GLfloat vertex_buffer_data[10010];
    int i = 0;
    float theta = 90 + angle;
    while(theta < 270 + angle){
        vertex_buffer_data[i] = major * cos((theta * PI) / 180.0);
        vertex_buffer_data[i+1] = minor * sin((theta * PI) / 180.0);
        vertex_buffer_data[i+2] = 0.0;
        vertex_buffer_data[i+3] = 0;
        vertex_buffer_data[i+4] = 0;
        vertex_buffer_data[i+5] = 0;
        theta += 1.0;
        vertex_buffer_data[i+6] = major * cos(theta * PI / 180.0);
        vertex_buffer_data[i+7] = minor * sin(theta * PI / 180.0);
        vertex_buffer_data[i+8] = 0;
        i += 9;
    }

    this->object = create3DObject(GL_TRIANGLES, i/3, vertex_buffer_data, color, GL_FILL);
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Magnet::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

// This detect is only for horizontal magnet
bool Magnet::detect(Ball Player){
    if(fabs(Player.position.x - this->x0) < MAGNET_RANGE && fabs(Player.position.y - this->y0) < (this->minor / 2)){
        if(fabs(angle) <= 90 && Player.position.x > this->x0)
            return 1;
        else if(fabs(angle) > 90 && Player.position.x < this->x0)
            return 1;
    }
    return 0;
}
