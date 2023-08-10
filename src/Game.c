#include "../Headers/Game.h"


int initialize_surface_map(SDL_Surface **surface_map)
{
    //unnecessary because surface_map is on stack but safety first i guess
    if(!surface_map)
    {
        return 1;
    }
    

    for(size_t i = 0; i < SIZE_SURFACE_MAP; ++i)
    {
        surface_map[i] = NULL;
    }

    surface_map[PLAYER_RIGHT_DOWN_SURF] = IMG_Load("Ressources/Player/PlayerRight.png");
    surface_map[PLAYER_RIGHT_UP_SURF] = IMG_Load("Ressources/Player/PlayerRightUp.png");
    surface_map[PLAYER_LEFT_DOWN_SURF] = IMG_Load("Ressources/Player/PlayerLeft.png");
    surface_map[PLAYER_LEFT_UP_SURF] = IMG_Load("Ressources/Player/PlayerLeft.png");
    surface_map[PLATFORM_SURF] = IMG_Load("Ressources/Assets/Platform.png");

    //check if every surface is initialized correctly
    for(size_t i = 0; i < SIZE_SURFACE_MAP; ++i)
    {
        if(!surface_map[i])
        {
            return 1;
        }
    }
    return 0;
}

void free_surface_map(SDL_Surface** surface_map)
{
    for(size_t i = 0; i < SIZE_SURFACE_MAP; ++i)
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

    SDL_Window *window = SDL_CreateWindow("Name", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, 0);
    SDL_Renderer *rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Surface* surface_map[SIZE_SURFACE_MAP];
    
    if(initialize_surface_map(surface_map))
    {
        fprintf(stderr, "Error initializing surface map\n");
        free_surface_map(surface_map);
        return 1;
    }
    
    SDL_Texture *player_right_down = SDL_CreateTextureFromSurface(rend, surface_map[PLAYER_RIGHT_DOWN_SURF]);
    
    SDL_Texture *player_left_down = SDL_CreateTextureFromSurface(rend, surface_map[PLAYER_LEFT_DOWN_SURF]);
    

    SDL_Rect player_rec;

    player_rec.w = 64;
    player_rec.h = 64;
    player_rec.x = 50;
    player_rec.y = 50;

    if (!window)
    {
        fprintf(stderr, "Error window %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    Player player =
        {50, 50, 0};
    Level level1 ={NULL, 0};

    Gameboard gameboard = {&player, NULL, 0};
    int running = 1;
    int up = 0;
    int left = 0;
    int down = 0;
    int right = 0;
    int dx = 0;
    int dy = 0;
    SDL_Texture *current_player_texture = player_right_down;

    
    while (running)
    {

        SDL_Event event;
        handle_input(&event, &up, &down, &right, &left, &running);

        dx = 0;
        dy = 0;
        if (up && !down)
        {
            dy = -SPEED;
        }
        if (down && !up)
        {
            dy = SPEED;
        }
        if (left && !right)
        {
            player.direction = 1;
            dx = -SPEED;
        }
        if (right && !left)
        {
            player.direction = 0;
            dx = SPEED;
        }

        player.x += dx;
        player.y += dy;

        player_rec.x = player.x;
        player_rec.y = player.y;

        current_player_texture = player.direction ? player_left_down : player_right_down;

        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, current_player_texture, NULL, &player_rec);
        SDL_RenderPresent(rend);
        SDL_Delay(1000 / 60);
    }

    SDL_DestroyTexture(player_right_down);
    SDL_DestroyTexture(player_left_down);
    
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}