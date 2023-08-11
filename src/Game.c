#include "../Headers/Game.h"


void render_player(SDL_Renderer* renderer, Player* player)
{
    SDL_RenderCopy(renderer, player->player_texture_map[player->current_texture], NULL, player->rect);
}

void render_level(SDL_Renderer* renderer, Level* level)
{
    for(size_t i = 0; i < level->platforms_size; ++i)
    {
        SDL_RenderCopy(renderer, level->platforms[i].texture, NULL, level->platforms[i].rect);
    }
}

void render(Game* game)
{
    SDL_RenderClear(game->renderer);
    render_level(game->renderer, &game->gameboard->levels[game->gameboard->current_level]);
    render_player(game->renderer, game->gameboard->player);
    
    SDL_RenderPresent(game->renderer);
}


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
    player->rect = player_rect;

    player->rect->w = 64;
    player->rect->h = 64;
    player->rect->x = 50;
    player->rect->y = 50;
    return 0;
}


void destroy_player(Player* player)
{
    for(size_t i = 0; i < SIZE_PLAYER_TEXTURE_MAP; ++i)
    {
        SDL_DestroyTexture(player->player_texture_map[i]);
    }

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
        fprintf(stderr, "Error initializing game\n");
        SDL_Quit();
        return 1;
    }

    game.surface_map = surface_map;
    
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

    Level levels[1] = {init_level1(surface_map[PLATFORM_SURF], game.renderer)};
    gameboard.levels = &levels; 
    gameboard.current_level = 0;

    game.running = 1;

    Controller controller = {0, 0, 0, 0};


    int timer_player_up_down = 0;

    //game loop
    while (game.running)
    {

        SDL_Event event;
        handle_input(&event, &controller, &game.running);
        move_player(&player, &controller);
        choose_player_texture(&player, &timer_player_up_down);
        

        render(&game);
        SDL_Delay(1000 / 60);
    }

    
    destroy_player(&player);
    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.window);
    
    SDL_Quit();
    return 0;
}