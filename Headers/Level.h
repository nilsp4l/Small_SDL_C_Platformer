#ifndef LEVEL_H
#define LEVEL_H

#include "Platform.h"
#include <stdlib.h>
#include "Enemy.h"


struct coin
{
    SDL_Rect *rect;
    int collected;
};

typedef struct coin Coin;

struct level
{
    Platform* platforms;
    size_t platforms_size;
    Enemy* enemies;
    size_t enemies_size;
    Coin* coins;
    size_t coins_size;
};

typedef struct level Level;

#endif