
#include "Projectile.h"
#include "SDL2/SDL.h"

#define SIZE_ENEMY_TEXTURE_MAP (2)
#define ENEMY_NOT_ATTACK_TEX (0)
#define ENEMY_ATTACK_TEX (1)


struct enemy
{
    Projectile* projectile;
    SDL_Texture** enemy_texture_map;
    SDL_Rect* rect;
    int amount_projectiles;
};

typedef struct enemy Enemy;