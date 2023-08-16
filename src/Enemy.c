#include "../Headers/Enemy.h"

void omega_wiggle(SDL_Rect *rect, int timer)
{
    if ((timer & 3) == 0)
    {
        rect->x -= 2;
    }
    else if ((timer & 1) == 0)
    {

        rect->x += 2;
    }
}

int start_shooting(Projectile *projectile)
{
    if (projectile->rect->w < PROJECTILE_SIZE && projectile->rect->h < PROJECTILE_SIZE)
    {
        ++projectile->rect->h;
        ++projectile->rect->w;
        omega_wiggle(projectile->rect, projectile->rect->w);
        return 0;
    }
    else
    {
        projectile->ready = 0;
        return 1;
    }
}

void update_enemy_projectile(Enemy *enemy)
{
    *enemy->current_projectile = use_clock(enemy->projectile_clock);

    // no projectile is ready
    if (!*enemy->current_projectile)
    {
        enemy->timer = 0;
    }
    else
    {

        (*(enemy->current_projectile))->rect->x = enemy->rect->x + PROJECTILE_SPAWN_OFFSET_X;
        (*(enemy->current_projectile))->rect->y = enemy->rect->y + PROJECTILE_SPANW_OFFSET_Y;
        (*(enemy->current_projectile))->rect->w = 0;
        (*(enemy->current_projectile))->rect->h = 0;
    }
}

void shoot(Enemy *enemy)
{

    if (!*enemy->current_projectile)
    {
        update_enemy_projectile(enemy);
    }

    ++enemy->timer;

    if (enemy->timer <= 100)
    {
        enemy->current_texture = ENEMY_NOT_ATTACK_TEX;
    }
    else if (enemy->timer > 100 && enemy->timer <= 200)
    {
        // wiggle the omega
        omega_wiggle(enemy->rect, enemy->timer);
    }
    else if (enemy->timer > 200)
    {
        enemy->current_texture = ENEMY_ATTACK_TEX;

        if (start_shooting(*enemy->current_projectile))
        {
            *enemy->current_projectile = NULL;
            enemy->timer = 0;
        }
    }
}

void let_enemies_attack(Enemy *enemies, size_t enemies_size)
{
    for (size_t i = 0; i < enemies_size; ++i)
    {
        shoot(enemies + i);
    }
}