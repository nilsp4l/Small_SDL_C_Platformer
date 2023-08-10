#include "SDL2/SDL.h"

void handle_input(SDL_Event *event, int *up, int *down, int *right, int *left, int *running)
{
    while (SDL_PollEvent(event))
    {
        switch (event->type)
        {
        case SDL_QUIT:
            *running = 0;
            break;
        case SDL_KEYDOWN:
            switch (event->key.keysym.scancode)
            {
            case SDL_SCANCODE_W:
                *up = 1;
                break;
            case SDL_SCANCODE_A:
                *left = 1;
                break;
            case SDL_SCANCODE_S:
                *down = 1;
                break;
            case SDL_SCANCODE_D:
                *right = 1;
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:

            switch (event->key.keysym.scancode)
            {
            case SDL_SCANCODE_W:
                *up = 0;
                break;
            case SDL_SCANCODE_A:
                *left = 0;
                break;
            case SDL_SCANCODE_S:
                *down = 0;
                break;
            case SDL_SCANCODE_D:
                *right = 0;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}