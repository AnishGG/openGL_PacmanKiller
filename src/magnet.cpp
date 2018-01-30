#include "magnet.h"

Magnet::Magnet(float x, float y, double length, double breadth, double angle, double thickness, color_t color) {

    this->outer = Magnet_boiler(x, y, length, breadth, angle, color);
    this->inner = Magnet_boiler(x, y, length - thickness, breadth - thickness, angle, COLOR_BACKGROUND);
}

void Magnet::draw(glm::mat4 VP){
    this->outer.draw(VP);
    this->inner.draw(VP);
}

void Magnet::detect_in_range(Ball *Player){
    if(this->outer.detect(*Player)){
        if(fabs(this->outer.angle) < 90.0)
            (*Player).speed_x -= MAGNET_DEACCELARATION;
        else
            (*Player).speed_x += MAGNET_DEACCELARATION;
    }
}
