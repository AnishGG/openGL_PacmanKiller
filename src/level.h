#include "main.h"
#include "score.h"
#ifndef LEVEL_H
#define LEVEL_H


class Level
{
public:
    Level();
    void level_up();
    int get_level();
    int get_level_up_score();
    char *get_string(Score score);
private:
    int level;
    int level_up_score;
    char *save[10];
};

#endif // LEVEL_H
