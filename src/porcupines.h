#include "main.h"
#include "ball.h"
#include "score.h"

#ifndef PORCUPINE_H
#define PORCUPINE_H


class Porcupine {
public:
    Porcupine() {}
    Porcupine(double x, double y, double base, double height, color_t color);
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void set_speed(float speed);
    void tick();
    bool detect_collision(Ball Player, Score *score);
    void set_validity(bool valid);

    // All variables go here
    glm::vec3 position;
    float rotation;
    float speed;
    float range, x0, y0, height;
private:
    VAO *object;
    bool is_valid;
    double timer;
};

#endif // PORCUPINE_H
