#include "main.h"

#ifndef RECTANLE_H
#define RECTANGLE_H


class Rectangle {
public:
    Rectangle() {}
    Rectangle(Point p1, Point p2, Point p3, Point p4, color_t color);
    glm::vec3 position;
    float rotation;
    float speed;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void set_speed(float speed);
    void tick();
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // GROUND_H