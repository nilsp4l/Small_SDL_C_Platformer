#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "Control.h"
#include "Level.h"

void do_gravity(Player *player, Platform *platform_player_is_on);
Platform *check_for_ground(Player *player, Level *level);
void move_player(Player *player, Controller *controller, Level *level);
int jump(Player *player);
#endif
