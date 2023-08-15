#include "../Headers/Gameboard.h"


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

Level init_level1(SDL_Surface *plat_surf, SDL_Surface *enemy_attack_surf, SDL_Surface *enemy_not_attack_surf, SDL_Texture** enemy_texture_map, SDL_Surface* projectile_surface, SDL_Renderer *renderer)
{
    Level to_return;
    to_return.platforms_size = 0; //error case
    size_t amount_plats = 5;
    to_return.platforms = malloc(amount_plats * sizeof(Platform));

    for(size_t i = 0; i < to_return.platforms_size; ++i)
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
    for(size_t i = 0; i < amount_plats; ++i)
    {
        if(!to_return.platforms->texture)
        {
            fprintf(stderr, "Error creating platform texture\n");
            return to_return;
        }
    }
    

    to_return.enemies = malloc(sizeof(Enemy));
    to_return.enemies->rect = malloc(sizeof(SDL_Rect));
    to_return.enemies->enemy_texture_map = enemy_texture_map;
    to_return.enemies->enemy_texture_map[ENEMY_NOT_ATTACK_TEX] = SDL_CreateTextureFromSurface(renderer, enemy_not_attack_surf);
    to_return.enemies->enemy_texture_map[ENEMY_ATTACK_TEX] = SDL_CreateTextureFromSurface(renderer, enemy_attack_surf);
    to_return.enemies->rect->x = 50;
    to_return.enemies->rect->y = 100;
    to_return.enemies->rect->h = 48;
    to_return.enemies->rect->w = 48;
    to_return.enemies_size = 1;
    to_return.enemies->current_texture = ENEMY_NOT_ATTACK_TEX;
    to_return.enemies->amount_projectiles = 1;
    to_return.enemies->projectile_queue = malloc(sizeof(Projectile_Queue));
    to_return.enemies->projectile_queue->queue = malloc(sizeof(Projectile*));
    to_return.enemies->projectile_queue->queue[0] = malloc(sizeof(Projectile));
    to_return.enemies->projectile_queue->queue[0]->rect = malloc(sizeof(SDL_Rect));
    to_return.enemies->projectile_queue->queue[0]->rect->x = to_return.enemies->rect->x + 16;
    to_return.enemies->projectile_queue->queue[0]->rect->y = to_return.enemies->rect->y + 23;
    to_return.enemies->projectile_queue->queue[0]->rect->w = 0;
    to_return.enemies->projectile_queue->queue[0]->rect->h = 0;
    to_return.enemies->projectile_queue->queue[0]->ready = 1;
    to_return.enemies->projectile_queue->queue_size = 1;
    to_return.enemies->projectile_queue->queue[0]->texture = SDL_CreateTextureFromSurface(renderer, projectile_surface);
    
    to_return.platforms_size = amount_plats;

    return to_return;
}