#include "../Headers/Gameboard.h"

int check_collision(Player* player, Projectile* projectile)
{
    int projectile_x = projectile->rect->x;
    int projectile_y = projectile->rect->y;

    int player_x = player->rect->x;
    int player_y = player->rect->y;


    if(!projectile->ready && projectile_x >= player_x - 32 && projectile_x <= player_x + PLAYER_WIDTH - 32 && projectile_y <= player_y + PLAYER_HEIGHT - 10 && projectile_y >= player_y + 10)
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
            if(check_collision(player, &enemy->projectile_clock->clock[j]))
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
        SDL_DestroyTexture(level->platforms[i].texture);
    }

    free(level->platforms);
}

void create_platform(Level *level, SDL_Rect *rects, SDL_Renderer *renderer, SDL_Surface *plat_surf, int pos, int x, int y, int w, int h, int is_base)
{
    rects[pos].x = x;
    rects[pos].y = y;
    rects[pos].w = w;
    rects[pos].h = h;
    level->platforms[pos].is_base = is_base;
    level->platforms[pos].rect = &rects[pos];
    level->platforms[pos].texture = SDL_CreateTextureFromSurface(renderer, plat_surf);
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

Level init_level1(SDL_Surface *plat_surf, SDL_Surface *enemy_attack_surf, SDL_Surface *enemy_not_attack_surf, SDL_Texture **enemy_texture_map, SDL_Surface *projectile_surface, SDL_Renderer *renderer)
{
    Level to_return;
    to_return.platforms_size = 0; // error case
    size_t amount_plats = 5;
    to_return.platforms = malloc(amount_plats * sizeof(Platform));

    for (size_t i = 0; i < to_return.platforms_size; ++i)
    {
        to_return.platforms[i].rect = NULL;
        to_return.platforms[i].texture = NULL;
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

    create_platform(&to_return, rects, renderer, plat_surf, 0, -40, 587, 720, 100, 1);
    create_platform(&to_return, rects, renderer, plat_surf, 1, 40, 480, 200, 100, 0);
    create_platform(&to_return, rects, renderer, plat_surf, 2, 250, 400, 100, 100, 0);
    create_platform(&to_return, rects, renderer, plat_surf, 3, 30, 290, 50, 100, 0);
    create_platform(&to_return, rects, renderer, plat_surf, 4, 500, 200, 80, 100, 0);
    for (size_t i = 0; i < amount_plats; ++i)
    {
        if (!to_return.platforms->texture)
        {
            fprintf(stderr, "Error creating platform texture\n");
            return to_return;
        }
    }

    to_return.enemies_size = 1;
    to_return.enemies = malloc(to_return.enemies_size * sizeof(Enemy));
    to_return.enemies[0].rect = malloc(sizeof(SDL_Rect));
    to_return.enemies[0].enemy_texture_map = enemy_texture_map;
    to_return.enemies[0].enemy_texture_map[ENEMY_NOT_ATTACK_TEX] = SDL_CreateTextureFromSurface(renderer, enemy_not_attack_surf);
    to_return.enemies[0].enemy_texture_map[ENEMY_ATTACK_TEX] = SDL_CreateTextureFromSurface(renderer, enemy_attack_surf);
    to_return.enemies[0].rect->x = 50;
    to_return.enemies[0].rect->y = 100;
    to_return.enemies[0].rect->h = 48;
    to_return.enemies[0].rect->w = 48;
    
    to_return.enemies[0].current_texture = ENEMY_NOT_ATTACK_TEX;
    to_return.enemies[0].amount_projectiles = 2;
    to_return.enemies[0].projectile_clock = malloc(sizeof(Projectile_Clock));
    to_return.enemies[0].projectile_clock->clock = malloc(2 * sizeof(Projectile));

    to_return.enemies[0].projectile_clock->clock[0].rect = malloc(sizeof(SDL_Rect));
    to_return.enemies[0].projectile_clock->clock[0].rect->x = to_return.enemies[0].rect->x + 16;
    to_return.enemies[0].projectile_clock->clock[0].rect->y = to_return.enemies[0].rect->y + 23;
    to_return.enemies[0].projectile_clock->clock[0].rect->w = 0;
    to_return.enemies[0].projectile_clock->clock[0].rect->h = 0;
    to_return.enemies[0].projectile_clock->clock[0].ready = 1;
    to_return.enemies[0].projectile_clock->clock_size = 2;
    to_return.enemies[0].projectile_clock->hand = 0;
    to_return.enemies[0].projectile_clock->clock[0].speed = 2;
    to_return.enemies[0].projectile_clock->clock[0].texture = SDL_CreateTextureFromSurface(renderer, projectile_surface);
    to_return.enemies[0].projectile_clock->clock[0].direction = BOTTOM_RIGHT;

    to_return.enemies[0].timer = 0;

    to_return.enemies[0].projectile_clock->clock[1].rect = malloc(sizeof(SDL_Rect));
    to_return.enemies[0].projectile_clock->clock[1].rect->x = to_return.enemies[0].rect->x + 16;
    to_return.enemies[0].projectile_clock->clock[1].rect->y = to_return.enemies[0].rect->y + 23;
    to_return.enemies[0].projectile_clock->clock[1].rect->w = 0;
    to_return.enemies[0].projectile_clock->clock[1].rect->h = 0;
    to_return.enemies[0].projectile_clock->clock[1].ready = 1;
    to_return.enemies[0].projectile_clock->clock[1].speed = 2;
    to_return.enemies[0].projectile_clock->clock[1].direction = DOWN;
    to_return.enemies[0].projectile_clock->clock[1].texture = SDL_CreateTextureFromSurface(renderer, projectile_surface);

    to_return.enemies[0].current_projectile = malloc(sizeof(Projectile *));
    *to_return.enemies[0].current_projectile = NULL;



    /*
   
    to_return.enemies[1].rect = malloc(sizeof(SDL_Rect));
    to_return.enemies[1].enemy_texture_map = enemy_texture_map;
    to_return.enemies[1].enemy_texture_map[ENEMY_NOT_ATTACK_TEX] = SDL_CreateTextureFromSurface(renderer, enemy_not_attack_surf);
    to_return.enemies[1].enemy_texture_map[ENEMY_ATTACK_TEX] = SDL_CreateTextureFromSurface(renderer, enemy_attack_surf);
    to_return.enemies[1].rect->x = 100;
    to_return.enemies[1].rect->y = 580;
    to_return.enemies[1].rect->h = 48;
    to_return.enemies[1].rect->w = 48;
    to_return.enemies[1].current_texture = ENEMY_NOT_ATTACK_TEX;
    to_return.enemies[1].amount_projectiles = 1;
    to_return.enemies[1].projectile_clock = malloc(sizeof(Projectile_Clock));
    to_return.enemies[1].projectile_clock->clock = malloc(sizeof(Projectile));

    to_return.enemies[1].projectile_clock->clock[0].rect = malloc(sizeof(SDL_Rect));
    to_return.enemies[1].projectile_clock->clock[0].rect->x = to_return.enemies[0].rect->x + 40;
    to_return.enemies[1].projectile_clock->clock[0].rect->y = to_return.enemies[0].rect->y + 23;
    to_return.enemies[1].projectile_clock->clock[0].rect->w = 0;
    to_return.enemies[1].projectile_clock->clock[0].rect->h = 0;
    to_return.enemies[1].projectile_clock->clock[0].ready = 1;
    to_return.enemies[1].projectile_clock->clock_size = 1;
    to_return.enemies[1].projectile_clock->hand = 0;
    to_return.enemies[1].projectile_clock->clock[0].speed = 0;
    to_return.enemies[1].projectile_clock->clock[0].texture = SDL_CreateTextureFromSurface(renderer, projectile_surface);
    to_return.enemies[1].projectile_clock->clock[0].direction = RIGHT;

    to_return.enemies[1].timer = 0;
    to_return.enemies[1].current_projectile = malloc(sizeof(Projectile *));
    *to_return.enemies[1].current_projectile = NULL;
    */
    to_return.platforms_size = amount_plats;

    return to_return;
}