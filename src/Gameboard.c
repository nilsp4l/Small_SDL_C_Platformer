#include "../Headers/Gameboard.h"

Level init_level1(SDL_Surface *plat_surf, SDL_Renderer *renderer)
{
    Level to_return;

    size_t amount_plats = 2;
    to_return.platforms = malloc(amount_plats * sizeof(Platform));

    if (!to_return.platforms)
    {
        return to_return;
    }

    SDL_Rect *rects = malloc(amount_plats * sizeof(SDL_Rect));

    if (!rects)
    {
        return to_return;
    }

    rects[0].x = -40;
    rects[0].y = 587;
    rects[0].w = 720;
    rects[0].h = 100;

    to_return.platforms[0].rect = &rects[0];
    to_return.platforms[0].texture = SDL_CreateTextureFromSurface(renderer, plat_surf);


    rects[1].x = 40;
    rects[1].y = 400;
    rects[1].w = 200;
    rects[1].h = 100;

    to_return.platforms[1].rect = &rects[1];
    to_return.platforms[1].texture = SDL_CreateTextureFromSurface(renderer, plat_surf);

    to_return.platforms_size = amount_plats;

    return to_return;
}