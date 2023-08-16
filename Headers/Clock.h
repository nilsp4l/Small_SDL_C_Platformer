#ifndef CLOCK_H
#define CLOCK_H


#include "Projectile.h"


struct projectile_clock
{
    Projectile* clock;
    size_t clock_size;
    int hand;
};

typedef struct projectile_clock Projectile_Clock;


Projectile* use_clock(Projectile_Clock* clock);

#endif