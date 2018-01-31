#include "display.h"
#include "main.h"
#include <cstring>

Display::Display(){}

// Here x and y is the start of the string to be displayed
void Display::store(char *s, float x, float y) {
    len = strlen(s);
    for (int i = 0; i < len; i++) {
        digs[i] = Digit(s[i], x + 0.3*i, 3.5);
    }
}

void Display::draw(glm::mat4 VP) {
    for (int i = 0; i < len; i++) digs[i].draw(VP);
}
