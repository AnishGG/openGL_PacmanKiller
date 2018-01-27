#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color, double Radius);
    void attach_slab(Point* points, float theta, float length, float thickness);
    bool my_collision(Ball b);
    glm::vec3 position;
    float rotation;
    float Radius;
    bool isSlabAttached, jumped;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void jump();
    float jump_theta(float theta_slab);
    void deaccelerate();
    double speed_x;
    double speed_y;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
