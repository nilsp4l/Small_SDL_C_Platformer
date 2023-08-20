#include "../Headers/Gameboard.h"

int check_collision(SDL_Rect *rect1, Player *player)
{
    if (rect1->x + rect1->w - PLAYER_X_OFFSET_RIGHT - 10 >= player->rect->x && rect1->x - PLAYER_X_OFFSET_LEFT <= player->rect->x && rect1->y - rect1->h - 10 <= player->rect->y && rect1->y + rect1->h >= player->rect->y + 10)
    {
        return 1;
    }
    return 0;
}

void check_coin_collection(Player *player, Level *level)
{
    Coin *current = NULL;
    for (size_t i = 0; i < level->coins_size; ++i)
    {
        current = &level->coins[i];
        if (!current->collected && check_collision(current->rect, player))
        {
            current->collected = 1;
        }
    }
}

int check_for_player_dead(Player *player, Level *level)
{
    Enemy *enemy = NULL;
    for (size_t i = 0; i < level->enemies_size; ++i)
    {
        enemy = &level->enemies[i];
        if (check_collision(level->enemies[i].rect, player))
        {
            return 1;
        }
        for (size_t j = 0; j < enemy->projectile_clock->clock_size; ++j)
        {
            if (!level->enemies->projectile_clock->clock[j].ready && check_collision(level->enemies[i].projectile_clock->clock[j].rect, player))
            {
                return 1;
            }
        }
    }

    return 0;
}


int check_all_coins_collected(Coin *coins, size_t coins_size)
{
    for (size_t i = 0; i < coins_size; ++i)
    {
        if (!coins[i].collected)
        {
            return 0;
        }
    }
    return 1;
}

int check_if_player_escaping(Player *player, Level *level)
{
    return check_collision(level->escape->rect, player) && check_all_coins_collected(level->coins, level->coins_size);
}

int player_escaping(Player *player, Escape *escape)
{
    if (player->rect->x == escape->rect->x && player->rect->y == escape->rect->y)
    {
        player->escaping = 0;
        return 0;
    }

    if (player->rect->y < escape->rect->y)
    {
        ++player->rect->y;
    }

    if (player->rect->y > escape->rect->y)
    {
        --player->rect->y;
    }

    if (player->rect->x < escape->rect->x)
    {
        ++player->rect->x;
    }

    if (player->rect->x > escape->rect->x)
    {
        --player->rect->x;
    }

    return 1;
}

void move_projectile(Projectile *projectile)
{
    switch (projectile->direction)
    {
    case UP:
        projectile->rect->y -= projectile->speed;
        break;
    case RIGHT:
        projectile->rect->x += projectile->speed;
        break;
    case DOWN:
        projectile->rect->y += projectile->speed;
        break;
    case LEFT:
        projectile->rect->x -= projectile->speed;
        break;
    case UP_RIGHT:
        projectile->rect->x += projectile->speed;
        projectile->rect->y -= projectile->speed;
        break;
    case UP_LEFT:
        projectile->rect->x -= projectile->speed;
        projectile->rect->y -= projectile->speed;
        break;
    case BOTTOM_RIGHT:
        projectile->rect->x += projectile->speed;
        projectile->rect->y += projectile->speed;
        break;
    case BOTTOM_LEFT:
        projectile->rect->x -= projectile->speed;
        projectile->rect->y += projectile->speed;
        break;
    default:
        break;
    }
}

void move_projectiles(Level *level)
{
    Projectile *current = NULL;
    for (size_t i = 0; i < level->enemies_size; ++i)
    {
        for (size_t j = 0; j < level->enemies[i].projectile_clock->clock_size; ++j)
        {
            current = &level->enemies[i].projectile_clock->clock[j];
            // check if ready projectile is out of bounds so it may be resetted for the enemy
            if (!current->ready && (current->rect->x > WINDOW_X_MAX || current->rect->x < WINDOW_X_OFFSET_TO_DISAPPEAR || current->rect->y > WINDOW_Y_MAX || current->rect->y < WINDOW_X_OFFSET_TO_DISAPPEAR))
            {
                current->ready = 1;
            }

            else if (!current->ready)
            {
                move_projectile(current);
            }
        }
    }
}



