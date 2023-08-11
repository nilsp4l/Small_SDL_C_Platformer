#include "../Headers/Player.h"
 
 
 void choose_player_texture(Player* player, int *timer)
 {
    player->current_texture = player->direction ? PLAYER_LEFT_DOWN_TEX : PLAYER_RIGHT_DOWN_TEX;
 }
