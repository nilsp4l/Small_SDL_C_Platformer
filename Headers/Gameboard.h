#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Player.h"
#include "Level.h"
#include <stdlib.h>

struct gameboard
{
    Player *player;
    Level *levels;
    size_t current_level;
};

typedef struct gameboard Gameboard;

void tear_down_level(Level* level);

Level init_level1(SDL_Surface *plat_surf, SDL_Renderer* renderer);

#endif