#include "main.h"
#include "ball.h"

#ifndef MAGNET_BOILER_H
#define MAGNET_BOILER_H


class Magnet_boiler {
public:
    Magnet_boiler() {}
    Magnet_boiler(float x, float y, double length, double breadth, double angle, color_t color);
    glm::vec3 position;
    float rotation;
    float major, minor, angle;
    float x0, y0;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bool detect(Ball Player);
private:
    VAO *object;
    const static double MAGNET_RANGE = 2.0;
};

#endif // MAGNET_H
