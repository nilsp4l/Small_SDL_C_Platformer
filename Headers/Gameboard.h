#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Player.h"
#include "Level.h"
#include "Constants.h"

#include <stdlib.h>



struct gameboard
{
    Player *player;
    Level *levels;
    size_t current_level;
};

typedef struct gameboard Gameboard;

void tear_down_level(Level *level);

Level init_level1(SDL_Surface **surface_map , SDL_Texture **enemy_texture_map, SDL_Renderer *renderer);
void move_projectiles(Level *level);
int check_collision(Player* player, Projectile* projectile);
int check_for_player_dead(Player *player, Level *level);

#endif