#include "main.h"
#include "ellipse.h"
#include "rectangle.h"

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
    bool isSlabAttached, jumped, drowned;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(Ellipse pool, Rectangle grass);
    void jump();
    float jump_theta(float theta_slab);
    void deaccelerate();
    double speed_x;
    double speed_y;
    bounding_box_t bounding_box();

private:
    VAO *object;
    // All constants go here
    static const float AIR_JUMP_SPEED = 0.20;
    static const float WATER_JUMP_SPEED = 0.10;
    static const float AIR_TERMINAL_SPEED = -0.15;
    static const float AIR_DEACCELARATION = 0.01;   // This is the deaccelaration per frame(60 frames per second).
    static const float WATER_AIR_EDGE_SPEED = -0.015;
};

#endif // BALL_H
