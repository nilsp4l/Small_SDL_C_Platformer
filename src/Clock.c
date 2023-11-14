#include "../Headers/Clock.h"

Projectile* use_clock(Projectile_Clock* clock)
{
    if(!clock)
    {
        return NULL;
    }

    Projectile* to_return = &clock->clock[clock->hand];
    // the clock will only be changed if the current element is ready otherwise the functionality of the clock wouldn't be the intended
    if(!to_return->ready)
    {
        return NULL;
    }
    
    //there are enemies which have no projectiles but still use the clock for simplicity
    if(clock->clock_size > 0)
    {
        clock->hand = (clock->hand + 1) % clock->clock_size;
    }
    
    
    return to_return;
}