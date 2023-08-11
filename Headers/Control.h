#ifndef CONTROL_H
#define CONTROL_H

struct controller
{
    int up;
    int down;
    int right;
    int left;
};

typedef struct controller Controller;

void handle_input(SDL_Event *event, Controller *controller, int *running);



#endif