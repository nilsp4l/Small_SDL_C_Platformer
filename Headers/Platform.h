#ifndef PLATFORM_H
#define PLATFORM_H
#include "SDL2/SDL.h"

struct platform
{
    SDL_Rect *rect;
    SDL_Texture *texture;
    int length;
    //only the platform on the bottom is base, flag is nessecary so player cant fall out of map
    int is_base; 
};

typedef struct platform Platform;

#endif