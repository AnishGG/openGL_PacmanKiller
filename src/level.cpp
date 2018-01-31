#include "level.h"
#include "main.h"
#include <cstring>

Level::Level()
{
    level = 1;
    level_up_score = 25;
    save[1] = "Level 1. Get 25 points in 40 seconds          score: ";
    save[2] = "Reached Level 2. Get 50 points in 40 seconds  score: ";
    save[3] = "Reached Level 3. Get 75 points in 40 seconds  score: ";
}

void Level::level_up(){
    level += 1;
    level_up_score += 25;
}

int Level::get_level(){
    return level;
}

int Level::get_level_up_score(){
    return level_up_score;
}

char* Level::get_string(Score score){
    char *s = (char*)std::malloc(sizeof(char)*1010);
    std::strcpy(s, save[this->get_level()]);
    char temp[10];
    sprintf(temp, "%d", score.get_score());
    std::strcat(s, temp);
    return s;
}
