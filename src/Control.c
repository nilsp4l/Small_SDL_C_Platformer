#include "SDL2/SDL.h"
#include "../Headers/Control.h"

void move_player(Player* player, Controller *controller)
{
        player->dx = 0;
        player->dy = 0;
        if (controller->up && !controller->down)
        {
            player->dy = -SPEED;
        }
        if (controller->down && !controller->up)
        {
            player->dy = SPEED;
        }
        if (controller->left && !controller->right)
        {
            player->direction = 1;
            player->dx = -SPEED;
        }
        if (controller->right && !controller->left)
        {
            player->direction = 0;
            player->dx = SPEED;
        }

        player->rect->x += player->dx;
        player->rect->y += player->dy;
        
}


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