#include "main.h"

#ifndef ELLIPSE_H
#define ELLIPSE_H


class Ellipse {
public:
    Ellipse() {}
    Ellipse(float x0, float y, color_t color, double major, double minor);
    float theta;
    float x_min;
    float x_max;
    float x0, y0;
    float a, b; // these are major and minor axis respectively
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    double calculate_y(float x);
private:
    VAO *object;
};

#endif // BALL_H
