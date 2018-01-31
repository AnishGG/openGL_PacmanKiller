#include "score.h"
#include "main.h"

Score::Score()
{
    score = 0;
}

void Score::update(int score) {
    this->score = score;
}

void Score::add(int n) {
    update(score + n);
}

void Score::subtract(int n) {
    update(score - n);
}

int Score::get_score(){
    return score;
}
