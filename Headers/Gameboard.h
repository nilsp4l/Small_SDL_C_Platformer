#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Player.h"
#include "Level.h"
#include "Constants.h"

#include <stdlib.h>




struct gameboard
{
    Player *player;
    Level *current_level;
};

typedef struct gameboard Gameboard;

void tear_down_level(Level *level);

int init_level1(Level* check);
void move_projectiles(Level *level);
void check_coin_collection(Player *player, Level *level);
int check_for_player_dead(Player *player, Level *level);
int check_if_player_escaping(Player *player, Level *level);
int player_escaping(Player* player, Escape* escape);
#endif