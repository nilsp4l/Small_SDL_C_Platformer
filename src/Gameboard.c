#include "../Headers/Gameboard.h"

int check_collision(Player *player, Projectile *projectile)
{
    int projectile_x = projectile->rect->x;
    int projectile_y = projectile->rect->y;

    int player_x = player->rect->x;
    int player_y = player->rect->y;

    if (!projectile->ready && projectile_x >= player_x + 16 && projectile_x <= player_x + PLAYER_WIDTH - 32 && projectile_y <= player_y + PLAYER_HEIGHT - 10 && projectile_y >= player_y + 10)
    {
        return 1;
    }

    return 0;
}

int check_for_player_dead(Player *player, Level *level)
{
    Enemy *enemy = NULL;
    for (size_t i = 0; i < level->enemies_size; ++i)
    {
        enemy = &level->enemies[i];
        for (size_t j = 0; j < enemy->projectile_clock->clock_size; ++j)
        {
            if (check_collision(player, &enemy->projectile_clock->clock[j]))
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

    free(level->platforms[0].rect);

    for (size_t i = 0; i < level->platforms_size; ++i)
    {
        // SDL_DestroyTexture(level->platforms[i].texture);
    }

    free(level->platforms);
}

void create_platform(Level *level, SDL_Rect *rects, int pos, int x, int y, int w, int h, int is_base)
{
    rects[pos].x = x;
    rects[pos].y = y;
    rects[pos].w = w;
    rects[pos].h = h;
    level->platforms[pos].is_base = is_base;
    level->platforms[pos].rect = &rects[pos];

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

int create_enemy(Enemy *enemies, int index, size_t enemies_size, Projectile *projectiles, int x, int y, size_t amount_projectiles)
{
    if (index >= enemies_size)
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

    enemies[index].projectile_clock->clock = malloc(amount_projectiles * sizeof(Projectile));

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

Level init_level1()
{
    int check = 0;

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

    SDL_Rect *rects = malloc(amount_plats * sizeof(SDL_Rect));

    if (!rects)
    {
        return to_return;
    }

    create_platform(&to_return, rects, 0, -40, 587, 720, 100, 1);
    create_platform(&to_return, rects, 1, 40, 480, 200, 100, 0);
    create_platform(&to_return, rects, 2, 250, 400, 100, 100, 0);
    create_platform(&to_return, rects, 3, 30, 350, 50, 100, 0);
    create_platform(&to_return, rects, 4, 500, 290, 80, 100, 0);
    

    to_return.enemies_size = 2;
    to_return.enemies = malloc(to_return.enemies_size * sizeof(Enemy));
    size_t amount_proj1 = 2;
    Projectile *enemy_1_projectiles = malloc(amount_proj1 * sizeof(Projectile));
    check |= create_projectile(enemy_1_projectiles, 0, DOWN, 2);
    check |= create_projectile(enemy_1_projectiles, 1, BOTTOM_RIGHT, 2);
    if (check)
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
    check |= create_projectile(enemy_2_projectiles, 0, BOTTOM_LEFT, 4);
    if (check)
    {
        puts("projectile 2");
        return to_return;
    }

    if(create_enemy(to_return.enemies, 1, to_return.enemies_size, enemy_2_projectiles, 580, 100, amount_proj2))
    {
        return to_return;
    }

    

    if (check)
    {
        puts("Error initializing level 1");
    }

    to_return.platforms_size = amount_plats;

    return to_return;
}