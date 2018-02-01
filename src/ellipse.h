#include "main.h"

#ifndef ELLIPSE_H
#define ELLIPSE_H


class Ellipse {
public:
    Ellipse() {}
    Ellipse(float x0, float y, color_t color, double major, double minor);
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    double calculate_y(float x);

    // All variables go here
    float theta;
    float x_min;
    float x_max;
    float a, b; // these are major and minor axis respectively
    glm::vec3 position;
    float rotation;
private:
    VAO *object;
};

#endif // BALL_H
