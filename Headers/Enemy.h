#include "SDL2/SDL.h"
#include "Player.h"
#include "Clock.h"

#define SIZE_ENEMY_TEXTURE_MAP (2)
#define ENEMY_NOT_ATTACK_TEX (0)
#define ENEMY_ATTACK_TEX (1)

#define PROJECTILE_SPAWN_OFFSET_X (16)
#define PROJECTILE_SPANW_OFFSET_Y (23)

struct enemy
{
    Projectile_Clock *projectile_clock;
    SDL_Texture **enemy_texture_map;
    SDL_Rect *rect;
    Projectile **current_projectile;
    size_t amount_projectiles;
    int current_texture;
    int timer;
};

typedef struct enemy Enemy;

void let_enemies_attack(Enemy *enemies, size_t enemies_size);

void shoot(Enemy *enemy);