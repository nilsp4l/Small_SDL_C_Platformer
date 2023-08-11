#ifndef PLATFORM_H
#define PLATFORM_H
#include "SDL2/SDL.h"

struct platform
{
    SDL_Rect *rect;
    SDL_Texture *texture;
    int length;
};

typedef struct platform Platform;

#endif