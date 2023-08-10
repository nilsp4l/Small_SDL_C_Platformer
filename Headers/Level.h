#ifndef LEVEL_H
#define LEVEL_H

#include "Platform.h"
#include <stdlib.h>

struct level
{
    Platform* platforms;
    size_t platforms_size;
};

typedef struct level Level;

#endif