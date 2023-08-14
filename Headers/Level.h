#ifndef LEVEL_H
#define LEVEL_H

#include "Platform.h"
#include <stdlib.h>
#include "Enemy.h"

struct level
{
    Platform* platforms;
    size_t platforms_size;
    Enemy* enemies;
    size_t enemies_size;
};

typedef struct level Level;

#endif