#include "../Headers/Gameboard.h"

Level* init_level1()
{
    Level* to_return = malloc(sizeof(Level));
    if(!to_return)
    {
        return NULL;
    }
    size_t amount_plats = 1;
    to_return->platforms = malloc(amount_plats * sizeof(Platform));
    if(!to_return)
    {
        return NULL;
    }

    to_return->platforms[0].x = 0;
    to_return->platforms[0].y = 620;
    to_return->platforms_size= amount_plats;

    return to_return;
}