#include "SDL2/SDL.h"
#include "Player.h"
#include "Clock.h"
#include "Constants.h"


#define PROJECTILE_SPAWN_OFFSET_X (16)
#define PROJECTILE_SPANW_OFFSET_Y (23)

struct enemy
{
    Projectile_Clock *projectile_clock;
    SDL_Rect *rect;
    Projectile **current_projectile;
    int current_texture;
    // must be a multiple of 20
    int cooldown;
    int timer;
};

typedef struct enemy Enemy;

void let_enemies_attack(Enemy *enemies, size_t enemies_size);

void shoot(Enemy *enemy);