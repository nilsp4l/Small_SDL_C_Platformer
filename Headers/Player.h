#ifndef PLAYER_H
#define PLAYER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_image.h"
#include "Constants.h"




// player rect size
#define PLAYER_WIDTH (64)
#define PLAYER_HEIGHT (64)

// some magic numbers because UI
#define PLAYER_Y_OFFSET (24)
#define PLAYER_X_OFFSET_LEFT (40)
#define PLAYER_X_OFFSET_RIGHT (16)

// coordinates of player starting point in the bottom left corner

#define PLAYER_X_START (10)
#define PLAYER_Y_START (563)

// player max jump in multiples of 8
#define PLAYER_MAX_JUMP_HEIGHT (15)

#define PLAYER_FLOATING_TIME (10)

struct player
{
    SDL_Rect *rect;
    int current_texture;
    int dx;
    int dy;
    int direction;
    int on_ground;
    int jump_progress;
    int timer;
    int escaping;
};

typedef struct player Player;

void choose_player_texture(Player *player);

#endif