#include "../Headers/Gameboard.h"

Level init_level1(SDL_Surface* plat_surf, SDL_Renderer* renderer)
{
    Level to_return;
    
    size_t amount_plats = 1;
    to_return.platforms = malloc(amount_plats * sizeof(Platform));
    
    if(!to_return.platforms)
    {
        return to_return;
    }
    SDL_Rect rect;
    rect.x = -40;
    rect.y = 587;
    rect.w = 720;
    rect.h = 100;
    
    to_return.platforms[0].rect = malloc(sizeof(SDL_Rect));
    if(!to_return.platforms)
    {
        return to_return;
    }

    *to_return.platforms[0].rect = rect;

    to_return.platforms[0].texture = SDL_CreateTextureFromSurface(renderer, plat_surf);

    to_return.platforms_size= amount_plats;

    return to_return;
}