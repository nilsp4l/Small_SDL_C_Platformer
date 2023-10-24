#include "../Headers/LevelFactory.h"

void free_platforms(Level *level)
{
    if (!level->platforms)
    {
        return;
    }

    for (size_t i = 0; i < level->platforms_size; ++i)
    {
        if(level->platforms[i].rect)
        {
            free(level->platforms[i].rect);
        }
    }

    free(level->platforms);
}

void free_coins(Level *level)
{
    if (!level->coins)
    {
        return;
    }

    for (size_t i = 0; i < level->coins_size; ++i)
    {
        free(level->coins[i].rect);
    }

    free(level->coins);
}

void free_enemies(Level *level)
{

    // the init level function has already failed at allocating the enemies -> no fields allocated
    if (!level->enemies)
    {
        return;
    }
    for (size_t i = 0; i < level->enemies_size; ++i)
    {
        if (level->enemies[i].projectile_clock && level->enemies[i].projectile_clock->clock)
        {
            for (size_t j = 0; j < level->enemies[i].projectile_clock->clock_size; j++)
            {
                free(level->enemies[i].projectile_clock->clock[j].rect);
            }
        }
    }

    for (size_t i = 0; i < level->enemies_size; ++i)
    {
        free(level->enemies[i].rect);
    }

    for (size_t i = 0; i < level->enemies_size; ++i)
    {
        if (level->enemies[i].projectile_clock)
        {
            free(level->enemies[i].projectile_clock->clock);
        }
    }

    for (size_t i = 0; i < level->enemies_size; ++i)
    {
        free(level->enemies[i].projectile_clock);
    }

    for (size_t i = 0; i < level->enemies_size; ++i)
    {
        free(level->enemies[i].current_projectile);
    }

    free(level->enemies);
}

void tear_down_level(Level *level)
{
    if (!level)
    {
        return;
    }

    free_coins(level);

    free_platforms(level);

    free_enemies(level);

    free(level->escape->rect);
    free(level->escape);
}


int create_platform(Level *level, int pos, int x, int y, int w, int h, int is_base)
{
    level->platforms[pos].rect = malloc(sizeof(SDL_Rect));
    if (!level->platforms[pos].rect)
    {
        return 1;
    }

    level->platforms[pos].rect->x = x;
    level->platforms[pos].rect->y = y;
    level->platforms[pos].rect->w = w;
    level->platforms[pos].rect->h = h;
    level->platforms[pos].is_base = is_base;

    return 0;
}

int create_projectile(Projectile *projectiles, int index, Direction direction, int speed)
{
    projectiles[index].rect = malloc(sizeof(SDL_Rect));
    if (!projectiles[index].rect)
    {
        return 1;
    }

    projectiles[index].rect->x = PROJECTILE_SIZE;
    projectiles[index].rect->y = PROJECTILE_SIZE;
    projectiles[index].rect->w = 0;
    projectiles[index].rect->h = 0;
    projectiles[index].ready = 1;
    projectiles[index].speed = speed;

    projectiles[index].direction = direction;
    return 0;
}

int create_enemy(Enemy *enemies, size_t index, size_t enemies_size, Projectile *projectiles, int x, int y, int cooldown, size_t amount_projectiles)
{

    if (index >= enemies_size)
    {
        return 1;
    }

    enemies[index].rect = malloc(sizeof(SDL_Rect));

    if (!enemies[index].rect)
    {
        return 1;
    }

    enemies[index].rect->x = x;
    enemies[index].rect->y = y;
    enemies[index].rect->h = 48;
    enemies[index].rect->w = 48;

    enemies[index].current_texture = ENEMY_NOT_ATTACK_TEX;
    enemies[index].projectile_clock = malloc(sizeof(Projectile_Clock));

    if (!enemies[index].projectile_clock)
    {
        return 1;
    }

    enemies[index].projectile_clock->clock = NULL;

    enemies[index].projectile_clock->clock_size = amount_projectiles;
    enemies[index].projectile_clock->hand = 0;
    enemies[index].projectile_clock->clock = projectiles;
    enemies[index].current_projectile = malloc(sizeof(Projectile *));

    if (!enemies[index].current_projectile)
    {
        return 1;
    }

    *enemies[index].current_projectile = NULL;

    enemies[index].timer = 0;
    enemies[index].cooldown = cooldown;
    SDL_Rect *current_proj_rect = NULL;
    for (size_t i = 0; i < enemies[index].projectile_clock->clock_size; ++i)
    {
        current_proj_rect = enemies[index].projectile_clock->clock[i].rect;
        current_proj_rect->x = enemies[index].rect->x + 16;
        current_proj_rect->y = enemies[index].rect->y + 23;
    }
    return 0;
}

int create_coin(Coin *coins, size_t index, size_t coins_size, int x, int y)
{
    if (index >= coins_size)
    {
        return 1;
    }

    coins[index].collected = 0;

    coins[index].rect = malloc(sizeof(SDL_Rect));

    if (!coins[index].rect)
    {
        return 1;
    }

    coins[index].rect->h = 16;
    coins[index].rect->w = 16;
    coins[index].rect->x = x;
    coins[index].rect->y = y;

    return 0;
}


void clear_level(Level *level)
{
    level->coins = NULL;
    level->coins_size = 0;
    level->enemies = NULL;
    level->enemies_size = 0;
    level->escape = NULL;
    level->platforms = NULL;
    level->platforms_size = 0;
}

void clear_enemies(Enemy *enemies, size_t enemies_size)
{
    if (!enemies)
    {
        return;
    }

    for (size_t i = 0; i < enemies_size; ++i)
    {
        enemies[i].current_projectile = NULL;
        enemies[i].current_texture = 0;
        enemies[i].projectile_clock = NULL;
        enemies[i].rect = NULL;
        enemies[i].timer = 0;
    }
}

void clear_platforms(Platform *platforms, size_t platforms_size)
{
    platforms_size = 0;
    for (size_t i = 0; i < platforms_size; ++i)
    {
        platforms[i].rect = NULL;
    }
}

Escape *create_escape()
{
    Escape *to_return = malloc(sizeof(Escape));
    if (!to_return)
    {
        return NULL;
    }

    to_return->rect = malloc(sizeof(SDL_Rect));

    if (!to_return->rect)
    {
        free(to_return);
        return NULL;
    }

    to_return->rect->x = 580;
    to_return->rect->y = 10;
    to_return->rect->w = 48;
    to_return->rect->h = 48;

    return to_return;
}

void print_parse_error(char current)
{
    fprintf(stderr, "Did not expect char %c in level file\n", current);
}

int parse_number(char* buffer, size_t buffer_size, char seperator, FILE* level_file)
{
    zero_array(buffer, buffer_size);
    char current = 0;
    for(size_t i = 0; i < buffer_size && current != EOF && current != seperator; ++i)
    {
        current = fgetc(level_file);
        *(buffer + i) = current;
    }

    return atoi(buffer);
}

void parse_coord_line(char* buffer, size_t buffer_size, int* coord_buffer, size_t coord_buffer_size, FILE* level_file)
{
    for(size_t i = 0; i < coord_buffer_size - 1; ++i)
    {
        *(coord_buffer + i) = 0;
        *(coord_buffer + i) = parse_number(buffer, buffer_size, ' ', level_file);
    }

    *(coord_buffer + coord_buffer_size - 1) = parse_number(buffer, buffer_size, '\n', level_file);
    
    
}

int parse_amount(FILE* level_file, char* buffer, size_t buffer_size, int *amount)
{
    char current = 0;
    
    
    zero_array(buffer, buffer_size);
    
    if(fgetc(level_file) != '\n' || fgetc(level_file) != '#')
    {
        print_parse_error(current);
        return 1;
    }
    int val = parse_number(buffer, buffer_size, '\n', level_file);
    

    if(val < 0 || val > 40)
    {
        fprintf(stderr, "Invalid amount\n");
        return 1;
    }

    *amount = val;
    
    return 0; 
}

int parse_platforms(FILE* level_file, Level* level, char* buffer, size_t buffer_size)
{
    

    int amount;

    parse_amount(level_file, buffer, buffer_size, &amount);

    level->platforms_size = amount + 1;
    

    level->platforms = malloc(level->platforms_size * sizeof(Platform));

    if (!level->platforms)
    {
        return 1;
    }
    clear_platforms(level->platforms, level->platforms_size);

    create_platform(level, 0, -40, 587, 720, 100, 1);


    int coord_buffer[4] = {0, 0, 0, 0};
    
    size_t counter = 1;

    for(; counter < level->platforms_size; ++counter)
    {
        parse_coord_line(buffer, buffer_size, (int*) coord_buffer, 4, level_file);
        if(create_platform(level, counter, coord_buffer[0], coord_buffer[1], coord_buffer[2], coord_buffer[3], 0))
        {
            return 1;
        }  
    }

    if(counter != level->platforms_size)
    {
        return 1;
    }

    if(fgetc(level_file) != ';' || fgetc(level_file) != '\n')
    {
        return 1;
    }


    return 0;
    
}

void parse_projectile_line(FILE* level_file, char* buffer, size_t buffer_size, Projectile* projectile, int index)
{
    zero_array(buffer, buffer_size);
    char current = 0;

    for(size_t i = 0; i < buffer_size && (current = fgetc(level_file)) != EOF && current != ' '; ++i)
    {
        buffer[i] = current;
    }
    
    Direction direction = DOWN;

    int dir = buffer[0] + buffer[1];

    switch(dir)
    {
        case 'B' : 
            direction = DOWN;
            break;
        case 'U' : 
            direction = UP;
            break;
        case 'L' : 
            direction = LEFT;
            break;
        case 'R' :
            direction = RIGHT;
            break;
        case 'U' + 'L' :
            direction = UP_LEFT;
            break;
        case 'U' + 'R' :
            direction = UP_RIGHT;
            break;
        case 'B' + 'L' :
            direction = BOTTOM_LEFT;
            break;
        case 'B' + 'R' :
            direction = BOTTOM_RIGHT;
            break;
        default:
            direction = DOWN;
            break;
    }
    

    zero_array(buffer, buffer_size);
    for(size_t i = 0; (current = fgetc(level_file)) != '\n'; ++i)
    {
        buffer[i] = current;
    }

    int speed = atoi(buffer);

    create_projectile(projectile, index, direction, speed);

}

int parse_projectiles(FILE* level_file, char* buffer, size_t buffer_size, Projectile** projectile_pointer, size_t* amount)
{
    zero_array(buffer, buffer_size);
    int val = 0;
    if(parse_amount(level_file, buffer, buffer_size, &val))
    {
        return 1;
    }

    *amount = val;

    *projectile_pointer = malloc(val * sizeof(Projectile));

    if(!*projectile_pointer)
    {
        return 1;
    }

    size_t counter = 0; 

    for(; counter < *amount; ++counter)
    {
        parse_projectile_line(level_file, buffer, buffer_size, *projectile_pointer, counter);
    }

    if(fgetc(level_file) != ';')
    {
        return 1;
    }
    if(counter != *amount)
    {
        return 1;
    }

    

    if(fgetc(level_file) != '\n')
    {
        return 1;
    }

    return 0;

}

int parse_enemies(FILE* level_file, Level* level, char* buffer, size_t buffer_size)
{
    zero_array(buffer, buffer_size);
    int amount;
    if(parse_amount(level_file, buffer, buffer_size, &amount))
    {
        return 1;
    }

    level->enemies_size = amount;
    level->enemies = malloc(level->enemies_size * sizeof(Enemy));
    if (!level->enemies)
    {
        return 1;
    }
    clear_enemies(level->enemies, level->enemies_size);

    size_t counter = 0;

    int coord_buffer[3] = {0, 0, 0};
    char current_char = 0;

    for(; counter < level->enemies_size; ++counter)
    {
        parse_coord_line(buffer, buffer_size, coord_buffer, 3, level_file);
        
        if((current_char = fgetc(level_file)) != 'J')
        {
            return 1;
        }

        Projectile* current = NULL;
        size_t proj_amount = 0;
        if(parse_projectiles(level_file, buffer, buffer_size, &current, &proj_amount))
        {
            return 1;
        }
        
        create_enemy(level->enemies, counter, level->enemies_size, current, coord_buffer[0], coord_buffer[1], coord_buffer[2], proj_amount);

        
    }

    if(counter != level->enemies_size)
    {
        return 1;
    }

    
    if(fgetc(level_file) != ';' || !((current_char = fgetc(level_file)) == '\n' || current_char == EOF))
    {
        return 1;
    }

    return 0;
}


int parse_coins(FILE* level_file, Level* level, char* buffer, size_t buffer_size)
{
    zero_array(buffer, buffer_size);
    int amount;
    if(parse_amount(level_file, buffer, buffer_size, &amount))
    {
        return 1;
    }

    level->coins_size = amount;
    level->coins = malloc(level->coins_size * sizeof(Coin));
    if (!level->coins)
    {
        return 1;
    }

    int coord_buffer[2] = {0, 0};
    
    size_t counter = 0;

    for(; counter < level->coins_size; ++counter)
    {
        parse_coord_line(buffer, buffer_size, (int*) coord_buffer, 2, level_file);
        if(create_coin(level->coins, counter, level->coins_size, coord_buffer[0], coord_buffer[1]))
        {
            return 1;
        }  
    }

    if(counter != level->coins_size)
    {
        return 1;
    }

    if(fgetc(level_file) != ';' || fgetc(level_file) != '\n')
    {
        return 1;
    }

    return 0;
    
}

int init_level_from_file(char* file_name, Level* level)
{
    clear_level(level);

    FILE* level_file = fopen(file_name, "r");
    if(!level_file)
    {
        fprintf(stderr, "Error opening level_file\n");
        return 1;
    }

    size_t buffer_size = 100;
    char buffer[buffer_size];
    zero_array(buffer, buffer_size);

    char current = 0;
    int platform_flag = 0;
    int enemy_flag = 0;
    int coin_flag = 0;
    while(current != EOF)
    {
        current = fgetc(level_file);
        switch(current)
        {
            case 'P': if(parse_platforms(level_file, level, buffer, buffer_size) || platform_flag)
            {
                fclose(level_file);
                return 1;
            }; platform_flag = 1; break;
            case 'E': if(parse_enemies(level_file, level, buffer, buffer_size) || enemy_flag)
            {
                fclose(level_file);
                return 1;
            }; enemy_flag = 1;
            break;
            case 'C': if(parse_coins(level_file, level, buffer, buffer_size) || coin_flag)
            {
                fclose(level_file);
                return 1;
            }; coin_flag = 1;
            break;
            default: break;
        }   
    }

    fclose(level_file);
    level->escape = create_escape();

    if (!level->escape)
    {
        return 1;
    }
    return 0;
}

int init_level(Level* level, int level_num)
{
    char buf[100] = {0};
    sprintf(buf, "Levels/%d.lvl", level_num);
    return init_level_from_file(buf, level);  
}

