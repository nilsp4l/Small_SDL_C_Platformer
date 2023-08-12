#ifndef PLAYER_H
#define PLAYER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_image.h"


//define texture positions inside the texture map of the player struct
#define SIZE_PLAYER_TEXTURE_MAP (4)
#define PLAYER_RIGHT_DOWN_TEX (0)
#define PLAYER_RIGHT_UP_TEX (1)
#define PLAYER_LEFT_DOWN_TEX (2)
#define PLAYER_LEFT_UP_TEX (3)

//player rect size
#define PLAYER_WIDTH (64)
#define PLAYER_HEIGHT (64)

//some magic numbers because UI
#define PLAYER_Y_OFFSET (24)
#define PLAYER_X_OFFSET_LEFT (38)
#define PLAYER_X_OFFSET_RIGHT (16)

struct player 
{
    SDL_Texture** player_texture_map;
    SDL_Rect* rect;
    int current_texture;
    int dx;
    int dy;
    int x;
    int y;
    int direction;
    int on_ground; 
};

typedef struct player Player;



void choose_player_texture(Player* player, int *timer);




#endif