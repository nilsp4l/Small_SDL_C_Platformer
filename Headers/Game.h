#ifndef GAME_H
#define GAME_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include "Gameboard.h"
#include "Control.h"

#define SPEED 4

int run_game();
int initialize_surface_map(SDL_Surface **surface_map);

// define surface positions inside the surface map
#define SIZE_SURFACE_MAP (5)
#define PLAYER_RIGHT_DOWN_SURF (0)
#define PLAYER_RIGHT_UP_SURF (1)
#define PLAYER_LEFT_DOWN_SURF (2)
#define PLAYER_LEFT_UP_SURF (3)
#define PLATFORM_SURF (4)

#endif