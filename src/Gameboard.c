#include "../Headers/Gameboard.h"

int check_collision(SDL_Rect *rect1, Player *player)
{
    if (rect1->x >= player->rect->x + rect1->w && rect1->x <= player->rect->x + PLAYER_WIDTH - 32 && rect1->y <= player->rect->y + PLAYER_HEIGHT - 10 && rect1->y >= player->rect->y + 10)
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
        if(!current->collected && check_collision(current->rect, player))
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

void tear_down_level(Level *level)
{
    if (!level)
    {
        return;
    }

    for(size_t i = 0; i < level->coins_size; ++i)
    {
        free(level->coins[i].rect);
        
    }
    
    free(level->coins);
    
    for(size_t i = 0; i < level->platforms_size; ++i)
    {
        free(level->platforms[i].rect);
    }

    

    
    free(level->platforms);
    
    

    for(size_t i = 0; i < level->enemies_size; ++i)
    {
        for(size_t j = 0; j < level->enemies[i].projectile_clock->clock_size; j++)
        {
            free(level->enemies[i].projectile_clock->clock[j].rect);
        }
    }


    for(size_t i = 0; i < level->enemies_size; ++i)
    {
        free(level->enemies[i].rect);
    }

    for(size_t i = 0; i < level->enemies_size; ++i)
    {
        free(level->enemies[i].projectile_clock->clock);
    }
    
    for(size_t i = 0; i < level->enemies_size; ++i)
    {
        free(level->enemies[i].projectile_clock);
    }
    
    for(size_t i = 0; i < level->enemies_size; ++i)
    {
        free(level->enemies[i].current_projectile);
    }

    
    free(level->enemies);
}

int create_platform(Level *level, int pos, int x, int y, int w, int h, int is_base)
{
    level->platforms[pos].rect = malloc(sizeof(Platform));
    if(!level->platforms[pos].rect)
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
    
    if(index >= enemies_size)
    {
        return 1;
    }
    
    enemies[index].rect = NULL;
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

    

    if (!enemies[index].projectile_clock->clock)
    {
        return 1;
    }

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

int create_coin(Coin* coins, size_t index, size_t coins_size, int x, int y)
{
    if(index >= coins_size)
    {
        return 1;
    }
    
    coins[index].collected = 0;

    coins[index].rect = malloc(sizeof(SDL_Rect));

    if(!coins[index].rect)
    {
        return 1;
    }

    coins[index].rect->h = 16;
    coins[index].rect->w = 16;
    coins[index].rect->x = x;
    coins[index].rect->y = y;

    return 0;
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

Level init_level1(int* check)
{
    

    Level to_return;
    to_return.platforms_size = 0; // error case
    size_t amount_plats = 5;
    to_return.platforms = malloc(amount_plats * sizeof(Platform));

    for (size_t i = 0; i < to_return.platforms_size; ++i)
    {
        to_return.platforms[i].rect = NULL;
    }

    if (!to_return.platforms)
    {
        return to_return;
    }

    

    

    *check |= create_platform(&to_return, 0, -40, 587, 720, 100, 1);
    *check |= create_platform(&to_return, 1, 40, 480, 200, 100, 0);
    *check |= create_platform(&to_return, 2, 250, 400, 100, 100, 0);
    *check |= create_platform(&to_return, 3, 30, 350, 50, 100, 0);
    *check |= create_platform(&to_return, 4, 500, 290, 80, 100, 0);

    if(*check)
    {
        return to_return;
    }

    to_return.enemies_size = 2;
    to_return.enemies = malloc(to_return.enemies_size * sizeof(Enemy));
    size_t amount_proj1 = 2;
    Projectile *enemy_1_projectiles = malloc(amount_proj1 * sizeof(Projectile));
    *check |= create_projectile(enemy_1_projectiles, 0, DOWN, 2);
    *check |= create_projectile(enemy_1_projectiles, 1, BOTTOM_RIGHT, 2);
    if (*check)
    {
        puts("projectiles 1");
        return to_return;
    }

    if (create_enemy(to_return.enemies, 0, to_return.enemies_size, enemy_1_projectiles, 50, 100, amount_proj1))
    {
        puts("enemy 1");
        return to_return;
    }

    size_t amount_proj2 = 1;
    Projectile *enemy_2_projectiles = malloc(amount_proj2 * sizeof(Projectile));
    *check |= create_projectile(enemy_2_projectiles, 0, BOTTOM_LEFT, 2);
    if (*check)
    {
        puts("projectile 2");
        return to_return;
    }

    if (create_enemy(to_return.enemies, 1, to_return.enemies_size, enemy_2_projectiles, 580, 100, amount_proj2))
    {
        return to_return;
    }

    if (*check)
    {
        puts("Error initializing level 1");
        return to_return;
    }

    to_return.platforms_size = amount_plats;

    to_return.coins_size = 2;

    to_return.coins = malloc(to_return.coins_size * sizeof(Coin));
    

    create_coin(to_return.coins, 0, to_return.coins_size, 300, 400);


    

    to_return.coins[1].collected = 0;

    to_return.coins[1].rect = malloc(sizeof(SDL_Rect));

    to_return.coins[1].rect->h = 16;
    to_return.coins[1].rect->w = 16;
    to_return.coins[1].rect->x = 50;
    to_return.coins[1].rect->y = 350;

    return to_return;
}