#include "porcupines.h"
#include "main.h"
#include "score.h"

Porcupine::Porcupine(double x, double y, double base, double height, color_t color) {
    this->position = glm::vec3(0, 0, 0);
    this->rotation = 0;
    this->speed = -0.001;
    this->range = 1;
    this->x0 = x;
    this->y0 = y;
    this->height = height;
    this->is_valid = 1;
    timer = 0.0;

    GLfloat vertex_buffer_data[] = {
        x,          y,          0,
        x - base/2, y,          0,
        x,          y + height, 0,

        x,          y,          0,
        x + base/2, y,          0,
        x,          y + height, 0,
    };

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}

void Porcupine::draw(glm::mat4 VP) {
    if(is_valid){
        Matrices.model = glm::mat4(1.0f);
        glm::mat4 translate = glm::translate (this->position);    // glTranslatef
        glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
        rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
        Matrices.model *= (translate * rotate);
        glm::mat4 MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        draw3DObject(this->object);
    }
}

void Porcupine::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Porcupine::tick() {
    if(range > 0.001){
        this->position.x += this->speed;
        range -= 0.001;
    }
    else{
        if(this->speed > 0) this->speed = -0.001;
        else                this->speed =  0.001;
        range = 1;
    }
    if(!is_valid){
        timer += 1.0/60.0;
        if(timer >= 4.0){
            timer = 0;
            is_valid = 1;
        }
    }
}

void Porcupine::set_speed(float speed){
    this->speed = speed;
}

bool Porcupine::detect_collision(Ball Player, Score *score){
    if(fabs(Player.position.x - (this->x0+this->position.x)) < Player.Radius && fabs(Player.position.y - this->y0) < (Player.Radius + this->height)){
        if(is_valid)    (*score).subtract();
        is_valid = 0;
        return 1;
    }
    return 0;
}
