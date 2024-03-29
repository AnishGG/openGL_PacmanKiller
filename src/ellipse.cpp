#include "ellipse.h"
#include "main.h"

Ellipse::Ellipse(float x, float y, color_t color, double major, double minor) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->x_min = x - major;
    this->x_max = x + major;
    this->a = major;
    this->b = minor;
    GLfloat vertex_buffer_data[10010];
    int i = 0;
    this->theta = 180.0;
    while(theta < 360.0){

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

void Ellipse::draw(glm::mat4 VP) {
    Matrices.model      =     glm::mat4(1.0f);
    glm::mat4 translate =     glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    =     glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate              =     rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model     *=     (translate * rotate);
    glm::mat4 MVP       =     VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ellipse::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

// Given an x-coordinate on the ellipse, what is the corrosponding y-coordinate on the ellipse
double Ellipse::calculate_y(float x){
    float y0 = this->position.y;
    float x0 = this->position.x;
    return (y0 - fabs(sqrt(this->b * this->b * (1 - (double)(((x - x0)*(x - x0))/(this->a * this->a))))));
}
