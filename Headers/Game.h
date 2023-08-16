#ifndef GAME_H
#define GAME_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>

#include "Movement.h"
#include "Gameboard.h"
#include "Control.h"


struct game
{
    SDL_Window *window;
    SDL_Surface **surface_map;
    SDL_Renderer *renderer;
    Gameboard *gameboard;
    int running;
};

typedef struct game Game;







/*
    Initialize the technological aspects of the game.
    Namely renderer and window
*/
int initialize_game_tech(Game *game);

int run_game();
int initialize_surface_map(SDL_Surface **surface_map);

int initialize_player(Player *player, Game *game, SDL_Rect *player_rect, SDL_Texture **texture_map);

void render_player(SDL_Renderer* renderer, Player* player);

void render_level(SDL_Renderer* renderer, Level* level);


void render(Game* game);

#endif