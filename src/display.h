#ifndef DISPLAY_H
#define DISPLAY_H

#include "digit.h"

class Display
{
public:
    Display();
    void store(char *s, float x, float y);
    void draw(glm::mat4 VP);
private:
    int len;
    Digit digs[50];
};

#endif // DISPLAY_H
