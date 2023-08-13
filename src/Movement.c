#include "../Headers/Movement.h"

void move_player(Player *player, Controller *controller, Level *level)
{
    player->dx = 0;
    player->dy = 0;

    if (controller->jump_interrupted)
    {
        player->jump_progress = 0;
        controller->jump_interrupted = 0;
    }

    Platform *platform_player_is_on = check_for_ground(player, level);

    if (controller->up && !controller->down && player->jump_progress <= PLAYER_MAX_JUMP_HEIGHT + PLAYER_FLOATING_TIME && (platform_player_is_on || player->jump_progress > 0))
    {
        controller->jump_interrupted = jump(player);
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

    do_gravity(player, platform_player_is_on);

    // if player wants to get down on a platform
    if (controller->down && !controller->up && platform_player_is_on && !platform_player_is_on->is_base)
    {

        player->dy = SPEED;
    }

    player->rect->x += player->dx;
    player->rect->y += player->dy;
}

Platform *check_for_ground(Player *player, Level *level)
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
        if (current->rect && current->rect->y + SPEED - 1 >= player->rect->y + 24 && current->rect->y - SPEED - 1 <= player->rect->y + 24
            // offset of PLAYER_WIDTH is necessary because the origin is in the bottom left corner
            && current->rect->x + current->rect->w - PLAYER_X_OFFSET_RIGHT >= player->rect->x && current->rect->x - PLAYER_X_OFFSET_LEFT <= player->rect->x)
        {
            return current;
        }
    }
    return NULL;
}

void do_gravity(Player *player, Platform *platform_player_is_on)
{

    if (player->jump_progress)
    {
        return;
    }

    if (platform_player_is_on)
    {
        player->on_ground = 1;
        player->dy = 0;
        player->rect->y = platform_player_is_on->rect->y - 24;
    }
    else
    {
        player->dy = SPEED;
        player->on_ground = 0;
    }
}

int jump(Player *player)
{
        if(player->jump_progress < PLAYER_MAX_JUMP_HEIGHT)
        {
            // SPEED * 2 upwards
            player->dy = -(SPEED << 1);
            ++player->jump_progress;
            return 0;
        }
        else if(player->jump_progress < PLAYER_MAX_JUMP_HEIGHT + PLAYER_FLOATING_TIME)
        {
            ++player->jump_progress;
            return 0;
        }
        else
        {
            return 1;
        }
        
}