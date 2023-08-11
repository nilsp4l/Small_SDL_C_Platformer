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
    
    
};

typedef struct player Player;



void choose_player_texture(Player* player, int *timer);



#endif