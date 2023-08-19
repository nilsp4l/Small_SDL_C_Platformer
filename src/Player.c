#include "../Headers/Player.h"

void choose_player_texture(Player *player)
{

  //the player should only wiggle when on a platform. otherwise it would look weird in the air
  if (!player->on_ground)
  {
    player->current_texture = player->direction ? PLAYER_LEFT_DOWN_TEX : PLAYER_RIGHT_DOWN_TEX;
    player->timer = 0;
  }
  else if (player->on_ground && player->timer <= 40)
  {
    player->current_texture = player->direction ? PLAYER_LEFT_DOWN_TEX : PLAYER_RIGHT_DOWN_TEX;
  }
  else if (player->on_ground && player->timer > 40 && player->timer <= 80)
  {
    player->current_texture = player->direction ? PLAYER_LEFT_UP_TEX : PLAYER_RIGHT_UP_TEX;
  }
  else
  {
    player->timer = 0;
  }
}


