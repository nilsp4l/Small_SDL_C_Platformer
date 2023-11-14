#ifndef GAME_H
#define GAME_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>

#include "Movement.h"
#include "Control.h"
#include "LevelFactory.h"

#define NORMAL_GAMEPLAY_MODE (0)
#define ESCAPE_MODE (1)

#define START_LEVEL (1)
#define MAX_LEVEL (5)

struct game;
typedef struct game Game;

struct game_mode
{
    int (*execute_mode)(Game *);
};

typedef struct game_mode Game_Mode;




struct game
{
    SDL_Window *window;
    SDL_Surface **surface_map;
    SDL_Renderer *renderer;
    Gameboard *gameboard;
    Controller *controller;
    SDL_Texture **env_texture_map;
    Game_Mode *game_modes;
    size_t max_level;
    size_t current_level_number;
    int current_mode;
    int running;
};




/*
    Initialize the technological aspects of the game.
    Namely renderer and window
*/
int initialize_game_tech(Game *game);

int run_game();
int initialize_surface_map(SDL_Surface **surface_map);

int initialize_player(Player *player, SDL_Rect *player_rect);

void render_player(SDL_Renderer *renderer, Player *player, SDL_Texture **env_texture_map);

void render_level(SDL_Renderer *renderer, Level *level, SDL_Texture **env_texture_map);

void render(Game *game, SDL_Texture **env_texture_map);

#endif