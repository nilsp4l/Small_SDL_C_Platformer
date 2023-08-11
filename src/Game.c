#include "../Headers/Game.h"

int initialize_player(Player *player, Game *game, SDL_Rect *player_rect, SDL_Texture **player_texture_map)
{
    if (!player)
    {
        return 1;
    }
    player->player_texture_map = player_texture_map;

    for (size_t i = 0; i < SIZE_PLAYER_TEXTURE_MAP; ++i)
    {
        player->player_texture_map[i] = NULL;
    }

    player->player_texture_map[PLAYER_RIGHT_UP_TEX] = SDL_CreateTextureFromSurface(game->renderer, game->surface_map[PLAYER_RIGHT_UP_SURF]);
    player->player_texture_map[PLAYER_RIGHT_DOWN_TEX] = SDL_CreateTextureFromSurface(game->renderer, game->surface_map[PLAYER_RIGHT_DOWN_SURF]);
    player->player_texture_map[PLAYER_LEFT_UP_TEX] = SDL_CreateTextureFromSurface(game->renderer, game->surface_map[PLAYER_LEFT_UP_SURF]);
    player->player_texture_map[PLAYER_LEFT_DOWN_TEX] = SDL_CreateTextureFromSurface(game->renderer, game->surface_map[PLAYER_LEFT_DOWN_SURF]);

    for (size_t i = 0; i < SIZE_PLAYER_TEXTURE_MAP; ++i)
    {
        if (!player->player_texture_map[i])
        {
            return 1;
        }
    }

    player->dx = 0;
    player->dy = 0;
    player->current_texture = player->player_texture_map[PLAYER_RIGHT_DOWN_TEX];
    player->rect = player_rect;

    player->rect->w = 64;
    player->rect->h = 64;
    player->rect->x = 50;
    player->rect->y = 50;
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
    surface_map[PLAYER_LEFT_UP_SURF] = IMG_Load("Ressources/Player/PlayerLeft.png");
    surface_map[PLATFORM_SURF] = IMG_Load("Ressources/Assets/Platform.png");

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
    game->window = SDL_CreateWindow("Name", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, 0);

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
        fprintf(stderr, "Error initializing game %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    game.surface_map = surface_map;
    SDL_Texture *player_right_down = SDL_CreateTextureFromSurface(game.renderer, surface_map[PLAYER_RIGHT_DOWN_SURF]);

    SDL_Texture *player_left_down = SDL_CreateTextureFromSurface(game.renderer, surface_map[PLAYER_LEFT_DOWN_SURF]);
    SDL_Texture *player_texture_map[SIZE_PLAYER_TEXTURE_MAP];
    Player player;

    SDL_Rect player_rec;

    if (initialize_player(&player, &game, &player_rec, player_texture_map))
    {
        fprintf(stderr, "Error initializing player\n");
        return 1;
    }

    Gameboard gameboard = {&player, NULL, 0};

    game.gameboard = &gameboard;

    Level level1 = {NULL, 0};

    game.running = 1;

    Controller controller = {0, 0, 0, 0};

    int dx = 0;
    int dy = 0;
    SDL_Texture *current_player_texture = player_right_down;

    while (game.running)
    {

        SDL_Event event;
        handle_input(&event, &controller, &game.running);

        dx = 0;
        dy = 0;
        if (controller.up && !controller.down)
        {
            dy = -SPEED;
        }
        if (controller.down && !controller.up)
        {
            dy = SPEED;
        }
        if (controller.left && !controller.right)
        {
            player.direction = 1;
            dx = -SPEED;
        }
        if (controller.right && !controller.left)
        {
            player.direction = 0;
            dx = SPEED;
        }

        player.x += dx;
        player.y += dy;

        game.gameboard->player->rect->x = player.x;
        game.gameboard->player->rect->y = player.y;

        current_player_texture = player.direction ? player_left_down : player_right_down;

        SDL_RenderClear(game.renderer);
        SDL_RenderCopy(game.renderer, current_player_texture, NULL, &player_rec);
        SDL_RenderPresent(game.renderer);
        SDL_Delay(1000 / 60);
    }

    SDL_DestroyTexture(player_right_down);
    SDL_DestroyTexture(player_left_down);

    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.window);
    SDL_Quit();
    return 0;
}