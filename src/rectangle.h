#include "main.h"

#ifndef RECTANGLE_H
#define RECTANGLE_H


class Rectangle {
public:
    Rectangle() {}
    Rectangle(Point p1, Point p2, Point p3, Point p4, color_t color);
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void set_speed(float speed);
    void tick();
    void set_theta(float theta);

    // All variables go here
    glm::vec3 position;
    float rotation;
    float speed;
    float theta;
    float max_point;

private:
    VAO *object;
};

#endif // RECTANGLE_H
