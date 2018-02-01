#include "main.h"
#include "ball.h"
#include "magnet_boiler.h"

#ifndef MAGNET_H
#define MAGNET_H

class Magnet {
public:
    Magnet() {}
    Magnet(float x, float y, double length, double breadth, double angle, double thickness, color_t color);
    void draw(glm::mat4 VP);
    void detect_in_range(Ball *Player);
private:
    static const double MAGNET_DEACCELARATION = 0.04;
    Magnet_boiler outer, inner;
};

#endif // BOILER_H
