#include "../Headers/Movement.h"

void move_player(Player *player, Controller *controller)
{

    if (controller->up && !controller->down)
    {
        player->dy = -SPEED * 2;
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

void fall(Player *player, Level *level)
{
    Platform *current = NULL;
    for (size_t i = 0; i < level->platforms_size; ++i)
    {
        current = &level->platforms[i];

        /*
        check with offset of SPEED - 1 if the player is on a platform
        offset is nessecary because the player only moves in multiples of SPEED and could therefore if the
        y - position is not aligned on SPEED not recognize the platform
        */
        if (current->rect->y + SPEED - 1 >= player->rect->y + 24
        && current->rect->y - SPEED - 1 <= player->rect->y + 24 
        // offset of PLAYER_WIDTH is necessary because the origin is in the bottom left corner 
        && current->rect->x + current->rect->w - PLAYER_X_OFFSET_RIGHT >= player->rect->x 
        && current->rect->x - PLAYER_X_OFFSET_LEFT <= player->rect->x)
        {
            player->on_ground = 1;
            player->dy = 0;
            player->rect->y = current->rect->y - 24;
            return;
        }
    }
    player->dy = SPEED;
    player->on_ground = 0;
}