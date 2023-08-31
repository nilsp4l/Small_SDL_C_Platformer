#include "../Headers/LevelFactory.h"

void free_platforms(Level *level)
{
    if (!level->platforms)
    {
        return;
    }

    for (size_t i = 0; i < level->platforms_size; ++i)
    {
        free(level->platforms[i].rect);
    }

    free(level->platforms);
}

void free_coins(Level *level)
{
    if (!level->coins)
    {
        return;
    }

    for (size_t i = 0; i < level->coins_size; ++i)
    {
        free(level->coins[i].rect);
    }

    free(level->coins);
}

void free_enemies(Level *level)
{

    // the init level function has already failed at allocating the enemies -> no fields allocated
    if (!level->enemies)
    {
        return;
    }
    for (size_t i = 0; i < level->enemies_size; ++i)
    {
        if (level->enemies[i].projectile_clock && level->enemies[i].projectile_clock->clock)
        {
            for (size_t j = 0; j < level->enemies[i].projectile_clock->clock_size; j++)
            {
                free(level->enemies[i].projectile_clock->clock[j].rect);
            }
        }
    }

    for (size_t i = 0; i < level->enemies_size; ++i)
    {
        free(level->enemies[i].rect);
    }

    for (size_t i = 0; i < level->enemies_size; ++i)
    {
        if (level->enemies[i].projectile_clock)
        {
            free(level->enemies[i].projectile_clock->clock);
        }
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
}

void tear_down_level(Level *level)
{
    if (!level)
    {
        return;
    }

    free_coins(level);

    free_platforms(level);

    free_enemies(level);

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

int create_enemy(Enemy *enemies, size_t index, size_t enemies_size, Projectile *projectiles, int x, int y, int cooldown, size_t amount_projectiles)
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
    enemies[index].cooldown = cooldown;
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


void clear_level(Level *level)
{
    level->coins = NULL;
    level->coins_size = 0;
    level->enemies = NULL;
    level->enemies_size = 0;
    level->escape = NULL;
    level->platforms = NULL;
    level->platforms_size = 0;
}

void clear_enemies(Enemy *enemies, size_t enemies_size)
{
    if (!enemies)
    {
        return;
    }

    for (size_t i = 0; i < enemies_size; ++i)
    {
        enemies[i].current_projectile = NULL;
        enemies[i].current_texture = 0;
        enemies[i].projectile_clock = NULL;
        enemies[i].rect = NULL;
        enemies[i].timer = 0;
    }
}

void clear_platforms(Platform *platforms, size_t platforms_size)
{
    platforms_size = 0;
    for (size_t i = 0; i < platforms_size; ++i)
    {
        platforms[i].rect = NULL;
    }
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


int init_level0(Level *level)
{
    if (!level)
    {
        return 1;
    }

    clear_level(level);

    int check = 0;

    level->platforms_size = 7;
    level->platforms = malloc(level->platforms_size * sizeof(Platform));

    if (!level->platforms)
    {
        return 1;
    }

    clear_platforms(level->platforms, level->platforms_size);

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
    if (!level->enemies)
    {
        return 1;
    }
    clear_enemies(level->enemies, level->enemies_size);
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
        return 1;
    }

    if (create_enemy(level->enemies, 0, level->enemies_size, enemy_1_projectiles, 50, 100, 20, amount_proj1))
    {
        return 1;
    }

    size_t amount_proj2 = 1;
    Projectile *enemy_2_projectiles = malloc(amount_proj2 * sizeof(Projectile));
    check |= create_projectile(enemy_2_projectiles, 0, UP, 2);
    if (check)
    {
        return 1;
    }

    if (create_enemy(level->enemies, 1, level->enemies_size, enemy_2_projectiles, 300, 560, 100, amount_proj2))
    {
        return 1;
    }

    if (check)
    {
        puts("Error initializing level 1");
        return 1;
    }

    

    level->coins_size = 2;

    level->coins = malloc(level->coins_size * sizeof(Coin));

    create_coin(level->coins, 0, level->coins_size, 300, 400);
    create_coin(level->coins, 1, level->coins_size, 50, 350);

    level->escape = create_escape();

    if (!level->escape)
    {
        return 1;
    }
    return 0;
}

int init_level1(Level *level)
{
    if (!level)
    {
        return 1;
    }

    clear_level(level);

    int check = 0;

    level->platforms_size = 7;
    level->platforms = malloc(level->platforms_size * sizeof(Platform));

    if (!level->platforms)
    {
        return 1;
    }

    clear_platforms(level->platforms, level->platforms_size);

    check |= create_platform(level, 0, X_BASE_PLAT, Y_BASE_PLAT, W_BASE_PLAT, H_BASE_PLAT, 1);
    check |= create_platform(level, 1, 40, 480, 200, H_BASE_PLAT, 0);
    check |= create_platform(level, 2, 250, 400, 100, H_BASE_PLAT, 0);
    check |= create_platform(level, 3, 380, 350, 50, H_BASE_PLAT, 0);
    check |= create_platform(level, 4, 500, 290, 80, H_BASE_PLAT, 0);
    check |= create_platform(level, 5, 400, 200, 80, H_BASE_PLAT, 0);
    check |= create_platform(level, 6, 540, 100, 80, H_BASE_PLAT, 0);

    if (check)
    {
        return 1;
    }

    level->enemies_size = 0;
    

    level->coins_size = 0;


    level->escape = create_escape();

    if (!level->escape)
    {
        return 1;
    }
    return 0;
}

int init_level2(Level *level)
{
    if (!level)
    {
        return 1;
    }

    clear_level(level);

    int check = 0;

    level->platforms_size = 7;
    level->platforms = malloc(level->platforms_size * sizeof(Platform));

    if (!level->platforms)
    {
        return 1;
    }

    clear_platforms(level->platforms, level->platforms_size);

    check |= create_platform(level, 0, X_BASE_PLAT, Y_BASE_PLAT, W_BASE_PLAT, H_BASE_PLAT, 1);
    check |= create_platform(level, 1, 40, 480, 100, 100, 0);
    check |= create_platform(level, 2, 250, 400, 100, 100, 0);
    check |= create_platform(level, 3, 40, 350, 100, 100, 0);
    check |= create_platform(level, 4, 500, 290, 100, 100, 0);
    check |= create_platform(level, 5, 500, 200, 100, 100, 0);
    check |= create_platform(level, 6, 500, 100, 100, 100, 0);

    if (check)
    {
        return 1;
    }

    level->enemies_size = 0;
    

    level->escape = create_escape();

    if (!level->escape)
    {
        return 1;
    }
    return 0;
}

int init_level3(Level *level)
{
    if (!level)
    {
        return 1;
    }

    clear_level(level);

    int check = 0;

    level->platforms_size = 7;
    level->platforms = malloc(level->platforms_size * sizeof(Platform));

    if (!level->platforms)
    {
        return 1;
    }

    clear_platforms(level->platforms, level->platforms_size);

    check |= create_platform(level, 0, X_BASE_PLAT, Y_BASE_PLAT, W_BASE_PLAT, H_BASE_PLAT, 1);
    check |= create_platform(level, 1, 40, 480, 100, 100, 0);
    check |= create_platform(level, 2, 250, 400, 100, 100, 0);
    check |= create_platform(level, 3, 40, 350, 100, 100, 0);
    check |= create_platform(level, 4, 500, 290, 100, 100, 0);
    check |= create_platform(level, 5, 500, 200, 100, 100, 0);
    check |= create_platform(level, 6, 500, 100, 100, 100, 0);

    if (check)
    {
        return 1;
    }

    level->enemies_size = 0;


    level->coins_size = 2;

    level->coins = malloc(level->coins_size * sizeof(Coin));

    create_coin(level->coins, 0, level->coins_size, 540, 540);
    create_coin(level->coins, 1, level->coins_size, 50, 350);
    

    level->escape = create_escape();

    if (!level->escape)
    {
        return 1;
    }
    return 0;
}

int init_level5(Level *level)
{
    if (!level)
    {
        return 1;
    }

    clear_level(level);

    int check = 0;

    level->platforms_size = 9;
    level->platforms = malloc(level->platforms_size * sizeof(Platform));

    if (!level->platforms)
    {
        return 1;
    }

    clear_platforms(level->platforms, level->platforms_size);

    check |= create_platform(level, 0, X_BASE_PLAT, Y_BASE_PLAT, W_BASE_PLAT, H_BASE_PLAT, 1);
    check |= create_platform(level, 1, 340, 470, 30, 100, 0);
    check |= create_platform(level, 2, 290, 390, 40, 100, 0);
    check |= create_platform(level, 3, 30, 350, 50, 100, 0);
    check |= create_platform(level, 4, 500, 290, 80, 100, 0);
    check |= create_platform(level, 5, 400, 200, 80, 100, 0);
    check |= create_platform(level, 6, 540, 100, 80, 100, 0);
    check |= create_platform(level, 7, 540, 100, 80, 100, 0);
    check |= create_platform(level, 8, 540, 100, 80, 100, 0);

    if (check)
    {
        return 1;
    }

    level->enemies_size = 2;
    level->enemies = malloc(level->enemies_size * sizeof(Enemy));
    if (!level->enemies)
    {
        return 1;
    }
    clear_enemies(level->enemies, level->enemies_size);
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
        return 1;
    }

    if (create_enemy(level->enemies, 0, level->enemies_size, enemy_1_projectiles, 50, 100, 20, amount_proj1))
    {
        return 1;
    }

    size_t amount_proj2 = 1;
    Projectile *enemy_2_projectiles = malloc(amount_proj2 * sizeof(Projectile));
    check |= create_projectile(enemy_2_projectiles, 0, UP, 2);
    if (check)
    {
        return 1;
    }

    if (create_enemy(level->enemies, 1, level->enemies_size, enemy_2_projectiles, 300, 560, 100, amount_proj2))
    {
        return 1;
    }

    if (check)
    {
        puts("Error initializing level 1");
        return 1;
    }

    

    level->coins_size = 2;

    level->coins = malloc(level->coins_size * sizeof(Coin));

    create_coin(level->coins, 0, level->coins_size, 300, 400);
    create_coin(level->coins, 1, level->coins_size, 50, 350);

    level->escape = create_escape();

    if (!level->escape)
    {
        return 1;
    }
    return 0;
}