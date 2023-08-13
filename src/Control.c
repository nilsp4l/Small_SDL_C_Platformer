#include "SDL2/SDL.h"
#include "../Headers/Control.h"


void handle_input(SDL_Event *event, Controller* controller, int *running)
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
                controller->up = 1;
                break;
            case SDL_SCANCODE_A:
                controller->left = 1;
                break;
            case SDL_SCANCODE_S:
                controller->down = 1;
                break;
            case SDL_SCANCODE_D:
                controller->right = 1;
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:

            switch (event->key.keysym.scancode)
            {
            case SDL_SCANCODE_W:
                controller->up = 0;
                controller->jump_interrupted = 1;
                break;
            case SDL_SCANCODE_A:
                controller->left = 0;
                break;
            case SDL_SCANCODE_S:
                controller->down = 0;
                break;
            case SDL_SCANCODE_D:
                controller->right = 0;
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