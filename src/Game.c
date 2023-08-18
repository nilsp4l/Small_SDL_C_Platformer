#include "../Headers/Game.h"

void render_player(SDL_Renderer *renderer, Player *player, SDL_Texture **env_texture_map)
{
    SDL_RenderCopy(renderer, env_texture_map[player->current_texture], NULL, player->rect);
}

void render_level(SDL_Renderer *renderer, Level *level, SDL_Texture **env_texture_map)
{
    for (size_t i = 0; i < level->platforms_size; ++i)
    {
        if (level->platforms[i].rect)
        {
            SDL_RenderCopy(renderer, env_texture_map[PLATFORM_TEX], NULL, level->platforms[i].rect);
        }
    }

    for (size_t i = 0; i < level->enemies_size; ++i)
    {
        if (level->enemies[i].rect)
        {
            SDL_RenderCopy(renderer, env_texture_map[level->enemies[i].current_texture], NULL, level->enemies[i].rect);
        }
    }

    for (size_t i = 0; i < level->enemies_size; ++i)
    {
        for (size_t j = 0; j < level->enemies[i].projectile_clock->clock_size; ++j)
        {
            if (level->enemies[i].projectile_clock->clock[j].rect)
            {
                SDL_RenderCopy(renderer, env_texture_map[PROJECTILE_TEX], NULL, level->enemies[i].projectile_clock->clock[j].rect);
            }
        }
    }

    for (size_t i = 0; i < level->coins_size; ++i)
    {
        if (!level->coins[i].collected)
        {
            SDL_RenderCopy(renderer, env_texture_map[COIN_TEX], NULL, level->coins[i].rect);
        }
    }
}

void render(Game *game, SDL_Texture **env_texture_map)
{
    SDL_RenderClear(game->renderer);
    render_level(game->renderer, &game->gameboard->levels[game->gameboard->current_level], env_texture_map);
    render_player(game->renderer, game->gameboard->player, env_texture_map);

    SDL_RenderPresent(game->renderer);
}

int initialize_env_texture_map(SDL_Renderer *renderer, SDL_Texture **env_texture_map, SDL_Surface **surface_map)
{
    env_texture_map[PROJECTILE_TEX] = SDL_CreateTextureFromSurface(renderer, surface_map[PROJECTILE]);
    env_texture_map[PLATFORM_TEX] = SDL_CreateTextureFromSurface(renderer, surface_map[PLATFORM_SURF]);
    env_texture_map[ENEMY_ATTACK_TEX] = SDL_CreateTextureFromSurface(renderer, surface_map[ENEMY_ATTACK]);
    env_texture_map[ENEMY_NOT_ATTACK_TEX] = SDL_CreateTextureFromSurface(renderer, surface_map[ENEMY_NOT_ATTACK]);
    env_texture_map[PLAYER_RIGHT_UP_TEX] = SDL_CreateTextureFromSurface(renderer, surface_map[PLAYER_RIGHT_UP_SURF]);
    env_texture_map[PLAYER_RIGHT_DOWN_TEX] = SDL_CreateTextureFromSurface(renderer, surface_map[PLAYER_RIGHT_DOWN_SURF]);
    env_texture_map[PLAYER_LEFT_UP_TEX] = SDL_CreateTextureFromSurface(renderer, surface_map[PLAYER_LEFT_UP_SURF]);
    env_texture_map[PLAYER_LEFT_DOWN_TEX] = SDL_CreateTextureFromSurface(renderer, surface_map[PLAYER_LEFT_DOWN_SURF]);
    env_texture_map[COIN_SURF] = SDL_CreateTextureFromSurface(renderer, surface_map[COIN_TEX]);

    for (size_t i = 0; i < SIZE_ENV_TEXTURE_MAP; ++i)
    {
        if (!*env_texture_map)
        {
            return 1;
        }
    }
    return 0;
}

int initialize_player(Player *player, SDL_Rect *player_rect)
{
    if (!player)
    {
        return 1;
    }
    player->timer = 0;
    player->direction = 0;
    player->jump_progress = 0;
    player->dx = 0;
    player->dy = 0;
    player->rect = player_rect;

    player->rect->w = PLAYER_WIDTH;
    player->rect->h = PLAYER_HEIGHT;
    player->rect->x = PLAYER_X_START;
    player->rect->y = PLAYER_Y_START;
    player->on_ground = 1;
    return 0;
}

int initialize_surface_map(SDL_Surface **surface_map)
{
    // unnecessary because surface_map is on stack but safety first i guess
    if (!surface_map)
    {
        return 1;
    }

    for (size_t i = 0; i < SIZE_SURFACE_MAP; ++i)
    {
        surface_map[i] = NULL;
    }

    surface_map[PLAYER_RIGHT_DOWN_SURF] = IMG_Load("Ressources/Player/PlayerRight.png");
    surface_map[PLAYER_RIGHT_UP_SURF] = IMG_Load("Ressources/Player/PlayerRightUp.png");
    surface_map[PLAYER_LEFT_DOWN_SURF] = IMG_Load("Ressources/Player/PlayerLeft.png");
    surface_map[PLAYER_LEFT_UP_SURF] = IMG_Load("Ressources/Player/PlayerLeftUp.png");
    surface_map[PLATFORM_SURF] = IMG_Load("Ressources/Assets/Platform.png");
    surface_map[ENEMY_NOT_ATTACK] = IMG_Load("Ressources/Enemy/EnemyNotAttack.png");
    surface_map[ENEMY_ATTACK] = IMG_Load("Ressources/Enemy/EnemyAttack.png");
    surface_map[PROJECTILE] = IMG_Load("Ressources/Enemy/Projectile.png");
    surface_map[COIN_SURF] = IMG_Load("Ressources/Assets/Coin.png");

    // check if every surface is initialized correctly
    for (size_t i = 0; i < SIZE_SURFACE_MAP; ++i)
    {
        if (!surface_map[i])
        {
            return 1;
        }
    }
    return 0;
}

int initialize_game_tech(Game *game)
{
    if (!game)
    {
        return 1;
    }
    game->window = SDL_CreateWindow("Name", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_X_MAX, WINDOW_Y_MAX, 0);

    if (!game->window)
    {
        return 1;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!game->renderer)
    {
        SDL_DestroyWindow(game->window);
        return 1;
    }

    return 0;
}

void free_surface_map(SDL_Surface **surface_map)
{
    for (size_t i = 0; i < SIZE_SURFACE_MAP; ++i)
    {
        SDL_FreeSurface(surface_map[i]);
    }
}

void free_env_texture_map(SDL_Texture** env_texture_map)
{
    for(size_t i = 0; i < SIZE_ENV_TEXTURE_MAP; ++i)
    {
        SDL_DestroyTexture(env_texture_map[i]);
    }
}

int run_game()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
    {
        fprintf(stderr, "Error init %s\n", SDL_GetError());
        return 1;
    }

    SDL_Surface *surface_map[SIZE_SURFACE_MAP];

    if (initialize_surface_map(surface_map))
    {
        fprintf(stderr, "Error initializing surface map\n");
        free_surface_map(surface_map);
        return 1;
    }

    Game game;

    if (initialize_game_tech(&game))
    {
        fprintf(stderr, "Error initializing game\n");
        SDL_Quit();
        return 1;
    }

    game.surface_map = surface_map;

    Player player;

    SDL_Rect player_rec;

    SDL_Texture *env_texture_map[SIZE_ENV_TEXTURE_MAP];

    if (initialize_env_texture_map(game.renderer, env_texture_map, surface_map))
    {
        return 1;
    }

    if (initialize_player(&player, &player_rec))
    {
        fprintf(stderr, "Error initializing player\n");
        return 1;
    }

    Gameboard gameboard = {&player, NULL, 0};

    game.gameboard = &gameboard;
    int check = 0;
    Level levels[1] = {init_level1(&check)};
    gameboard.levels = (Level *)&levels;
    gameboard.current_level = 0;

    game.running = 1;

    Controller controller = {0, 0, 0, 0, 0};

    

    // game loop
    while (game.running)
    {
        ++player.timer;

        SDL_Event event;

        handle_input(&event, &controller, &game.running);

        move_player(&player, &controller, &game.gameboard->levels[game.gameboard->current_level]);
        let_enemies_attack(levels[game.gameboard->current_level].enemies, levels[game.gameboard->current_level].enemies_size);
        move_projectiles(&game.gameboard->levels[game.gameboard->current_level]);
        if (check_for_player_dead(&player, &game.gameboard->levels[game.gameboard->current_level]))
        {
            player.rect->x = PLAYER_X_START;
            player.rect->y = PLAYER_Y_START;
        }

        check_coin_collection(&player, &game.gameboard->levels[game.gameboard->current_level]);
        choose_player_texture(&player);

        render(&game, env_texture_map);
        SDL_Delay(1000 / 128);
    }

    tear_down_level(&gameboard.levels[gameboard.current_level]);
    free_surface_map(game.surface_map);
    free_env_texture_map(env_texture_map);
    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.window);

    SDL_Quit();
    return 0;
}