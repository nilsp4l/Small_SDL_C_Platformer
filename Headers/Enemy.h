

#include "SDL2/SDL.h"
#include "Player.h"
#include "Queue.h"

#define SIZE_ENEMY_TEXTURE_MAP (2)
#define ENEMY_NOT_ATTACK_TEX (0)
#define ENEMY_ATTACK_TEX (1)

struct enemy
{
    Projectile_Queue* projectile_queue;
    SDL_Texture **enemy_texture_map;
    SDL_Rect *rect;
    int current_texture;
    int amount_projectiles;
    int timer;
};

typedef struct enemy Enemy;

void let_enemies_attack(Enemy *enemies, size_t enemies_size, Player *player);

void shoot(Enemy *enemy, Player *player);