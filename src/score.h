#include "main.h"
#ifndef SCORE_H
#define SCORE_H


class Score
{
public:
    Score();
    void add(int n = 3);
    void subtract(int n = 1);
    int get_score();

private:
    int score;
    void update(int score);
};

#endif // SCORE_H
