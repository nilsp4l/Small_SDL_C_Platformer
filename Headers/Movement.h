#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "Control.h"
#include "Level.h"

void fall(Player* player, Level* level);
void move_player(Player* player, Controller *controller);

#endif

