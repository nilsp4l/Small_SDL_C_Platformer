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

Escape *create_escape()
{
    Escape *to_return = malloc(sizeof(Escape));
    if (!to_return)
    {
        return NULL;
    }

    to_return->rect = malloc(sizeof(SDL_Rect));

    if (!to_return->rect)
    {
        free(to_return);
        return NULL;
    }

    to_return->rect->x = 580;
    to_return->rect->y = 10;
    to_return->rect->w = 48;
    to_return->rect->h = 48;

    return to_return;
}

void tear_down_level(Level *level)
{
    if (!level)
    {
        return;
    }

    for (size_t i = 0; i < level->coins_size; ++i)
    {
        free(level->coins[i].rect);
    }

    free(level->coins);

    for (size_t i = 0; i < level->platforms_size; ++i)
    {
        free(level->platforms[i].rect);
    }

    free(level->platforms);

    for (size_t i = 0; i < level->enemies_size; ++i)
    {
        for (size_t j = 0; j < level->enemies[i].projectile_clock->clock_size; j++)
        {
            free(level->enemies[i].projectile_clock->clock[j].rect);
        }
    }

    for (size_t i = 0; i < level->enemies_size; ++i)
    {
        free(level->enemies[i].rect);
    }

    for (size_t i = 0; i < level->enemies_size; ++i)
    {
        free(level->enemies[i].projectile_clock->clock);
    }

    for (size_t i = 0; i < level->enemies_size; ++i)
    {
        free(level->enemies[i].projectile_clock);
    }

    for (size_t i = 0; i < level->enemies_size; ++i)
    {
        free(level->enemies[i].current_projectile);
    }

    free(level->enemies);

    free(level->escape);
}

int create_platform(Level *level, int pos, int x, int y, int w, int h, int is_base)
{
    level->platforms[pos].rect = malloc(sizeof(Platform));
    if (!level->platforms[pos].rect)
    {
        return 1;
    }

    level->platforms[pos].rect->x = x;
    level->platforms[pos].rect->y = y;
    level->platforms[pos].rect->w = w;
    level->platforms[pos].rect->h = h;
    level->platforms[pos].is_base = is_base;

    return 0;
}

int create_projectile(Projectile *projectiles, int index, Direction direction, int speed)
{
    projectiles[index].rect = malloc(sizeof(SDL_Rect));
    if (!projectiles[index].rect)
    {
        return 1;
    }

    projectiles[index].rect->x = PROJECTILE_SIZE;
    projectiles[index].rect->y = PROJECTILE_SIZE;
    projectiles[index].rect->w = 0;
    projectiles[index].rect->h = 0;
    projectiles[index].ready = 1;
    projectiles[index].speed = speed;

    projectiles[index].direction = direction;
    return 0;
}

int create_enemy(Enemy *enemies, size_t index, size_t enemies_size, Projectile *projectiles, int x, int y, size_t amount_projectiles)
{

    if (index >= enemies_size)
    {
        return 1;
    }

    enemies[index].rect = malloc(sizeof(SDL_Rect));

    if (!enemies[index].rect)
    {
        return 1;
    }

    enemies[index].rect->x = x;
    enemies[index].rect->y = y;
    enemies[index].rect->h = 48;
    enemies[index].rect->w = 48;

    enemies[index].current_texture = ENEMY_NOT_ATTACK_TEX;
    enemies[index].projectile_clock = malloc(sizeof(Projectile_Clock));

    if (!enemies[index].projectile_clock)
    {
        return 1;
    }

    enemies[index].projectile_clock->clock = NULL;

    enemies[index].projectile_clock->clock_size = amount_projectiles;
    enemies[index].projectile_clock->hand = 0;
    enemies[index].projectile_clock->clock = projectiles;
    enemies[index].current_projectile = malloc(sizeof(Projectile *));

    if (!enemies[index].current_projectile)
    {
        return 1;
    }

    *enemies[index].current_projectile = NULL;

    enemies[index].timer = 0;

    SDL_Rect *current_proj_rect = NULL;
    for (size_t i = 0; i < enemies[index].projectile_clock->clock_size; ++i)
    {
        current_proj_rect = enemies[index].projectile_clock->clock[i].rect;
        current_proj_rect->x = enemies[index].rect->x + 16;
        current_proj_rect->y = enemies[index].rect->y + 23;
    }
    return 0;
}

int create_coin(Coin *coins, size_t index, size_t coins_size, int x, int y)
{
    if (index >= coins_size)
    {
        return 1;
    }

    coins[index].collected = 0;

    coins[index].rect = malloc(sizeof(SDL_Rect));

    if (!coins[index].rect)
    {
        return 1;
    }

    coins[index].rect->h = 16;
    coins[index].rect->w = 16;
    coins[index].rect->x = x;
    coins[index].rect->y = y;

    return 0;
}

int check_all_coins_collected(Coin* coins, size_t coins_size)
{
    for(size_t i = 0; i < coins_size; ++i)
    {
        if(!coins[i].collected)
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


int player_escaping(Player* player, Escape* escape)
{
    if(player->rect->x == escape->rect->x && player->rect->y == escape->rect->y)
    {
        player->escaping = 0;
        return 0;
    }

    if(player->rect->y < escape->rect->y)
    {
        ++player->rect->y;
    }

    if(player->rect->y > escape->rect->y)
    {
        --player->rect->y;
    }

    if(player->rect->x < escape->rect->x)
    {
        ++player->rect->x;
    }

    if(player->rect->x > escape->rect->x)
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

int init_level1(Level *level)
{

    int check = 0;

    level->platforms_size = 0; // error case
    size_t amount_plats = 7;
    level->platforms = malloc(amount_plats * sizeof(Platform));

    for (size_t i = 0; i < level->platforms_size; ++i)
    {
        level->platforms[i].rect = NULL;
    }

    if (!level->platforms)
    {
        return 1;
    }

    check |= create_platform(level, 0, -40, 587, 720, 100, 1);
    check |= create_platform(level, 1, 40, 480, 200, 100, 0);
    check |= create_platform(level, 2, 250, 400, 100, 100, 0);
    check |= create_platform(level, 3, 30, 350, 50, 100, 0);
    check |= create_platform(level, 4, 500, 290, 80, 100, 0);
    check |= create_platform(level, 5, 400, 200, 80, 100, 0);
    check |= create_platform(level, 6, 540, 100, 80, 100, 0);

    if (check)
    {
        return 1;
    }

    level->enemies_size = 2;
    level->enemies = malloc(level->enemies_size * sizeof(Enemy));
    size_t amount_proj1 = 2;
    Projectile *enemy_1_projectiles = malloc(amount_proj1 * sizeof(Projectile));
    if (!enemy_1_projectiles)
    {
        return 1;
    }

    check |= create_projectile(enemy_1_projectiles, 0, DOWN, 2);
    check |= create_projectile(enemy_1_projectiles, 1, BOTTOM_RIGHT, 2);
    if (check)
    {
        puts("projectiles 1");
        return 1;
    }

    if (create_enemy(level->enemies, 0, level->enemies_size, enemy_1_projectiles, 50, 100, amount_proj1))
    {
        puts("enemy 1");
        return 1;
    }

    size_t amount_proj2 = 1;
    Projectile *enemy_2_projectiles = malloc(amount_proj2 * sizeof(Projectile));
    check |= create_projectile(enemy_2_projectiles, 0, BOTTOM_LEFT, 2);
    if (check)
    {
        puts("projectile 2");
        return 1;
    }

    if (create_enemy(level->enemies, 1, level->enemies_size, enemy_2_projectiles, 300, 560, amount_proj2))
    {
        return 1;
    }

    if (check)
    {
        puts("Error initializing level 1");
        return 1;
    }

    level->platforms_size = amount_plats;

    level->coins_size = 2;

    level->coins = malloc(level->coins_size * sizeof(Coin));

    create_coin(level->coins, 0, level->coins_size, 300, 400);
    create_coin(level->coins, 1, level->coins_size, 50, 350);

    level->escape = create_escape();

    if (!level->escape)
    {
        check = 1;
        return 1;
    }
    return 0;
}