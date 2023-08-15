#include "../Headers/Enemy.h"

void omega_wiggle(Enemy *enemy)
{
    if ((enemy->timer & 3) == 0)
    {
        enemy->rect->x -= 2;
    }
    else if ((enemy->timer & 1) == 0)
    {

        enemy->rect->x += 2;
    }
}

void start_shooting(Projectile *projectile)
{
    if (projectile->rect->w < PROJECTILE_SIZE && projectile->rect->h < PROJECTILE_SIZE)
    {
        ++projectile->rect->h;
        ++projectile->rect->w;
        if (projectile->rect->w & 1)
        {
            ++projectile->rect->x;
            ++projectile->rect->y;
        }
    }
    else
    {
        projectile->ready = 1;
    }
}

void shoot(Enemy *enemy, Player *player)
{

    ++enemy->timer;

    if (enemy->timer <= 100)
    {
        enemy->current_texture = ENEMY_NOT_ATTACK_TEX;
    }
    else if (enemy->timer > 100 && enemy->timer <= 200)
    {
        // wiggle the omega
        omega_wiggle(enemy);
    }
    else if (enemy->timer > 200 && enemy->timer <= 300)
    {
        enemy->current_texture = ENEMY_ATTACK_TEX;
        start_shooting(serve_first(enemy->projectile_queue));
    }
    else
    {
        enemy->timer = 0;
    }
}

void let_enemies_attack(Enemy *enemies, size_t enemies_size, Player *player)
{
    for (size_t i = 0; i < enemies_size; ++i)
    {
        shoot(enemies + i, player);
    }
}