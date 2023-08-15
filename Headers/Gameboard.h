#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Player.h"
#include "Level.h"

#include <stdlib.h>

#define WINDOW_X_MAX (640)
#define WINDOW_Y_MAX (640)

#define WINDOW_X_OFFSET_TO_DISAPPEAR (-40)

struct gameboard
{
    Player *player;
    Level *levels;
    size_t current_level;
};

typedef struct gameboard Gameboard;

void tear_down_level(Level *level);

Level init_level1(SDL_Surface *plat_surf, SDL_Surface *enemy_attack_surf, SDL_Surface *enemy_not_attack_surf, SDL_Texture** enemy_texture_map, SDL_Surface* projectile_surface, SDL_Renderer *renderer);

#endif