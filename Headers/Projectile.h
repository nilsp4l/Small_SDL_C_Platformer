#include "SDL2/SDL.h"

#define PROJECTILE_SIZE (16)

enum direction 
{
    UP, RIGHT, LEFT, DOWN
};

typedef enum direction Direction;

struct projectile
{
    SDL_Rect *rect;
    SDL_Texture *texture;
    int ready;
    int speed;
};

typedef struct projectile Projectile;