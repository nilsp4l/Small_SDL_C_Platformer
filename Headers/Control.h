#ifndef CONTROL_H
#define CONTROL_H
#include "Player.h"

#define SPEED 4

struct controller
{
    int up;
    int down;
    int right;
    int left;
};

typedef struct controller Controller;

void handle_input(SDL_Event *event, Controller *controller, int *running);

void move_player(Player* player, Controller *controller);

#endif