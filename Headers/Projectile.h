#include "SDL2/SDL.h"

#define PROJECTILE_SIZE (16)

enum direction 
{
    UP, RIGHT, LEFT, DOWN, UP_RIGHT, BOTTOM_RIGHT, UP_LEFT, BOTTOM_LEFT
};

typedef enum direction Direction;

struct projectile
{
    SDL_Rect *rect;
    SDL_Texture *texture;
    int ready;
    int speed;
    Direction direction;
};

typedef struct projectile Projectile;