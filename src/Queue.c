#include "../Headers/Queue.h"

int push_back(Projectile_Queue* queue, Projectile* to_insert)
{
    if(!(queue && to_insert))
    {
        return 1;
    }

    queue->queue[queue->queue_size - 1] = to_insert;
    
    return 0;
}


Projectile* pop_front(Projectile_Queue* queue)
{
    if(!queue)
    {
        return NULL;
    }

    Projectile* to_return = queue->queue[0];
    for(size_t i = 0; i < queue->queue_size - 1; ++i)
    {
        queue->queue[i] = queue->queue[i + 1];
    }
    
    return to_return;
}

Projectile* serve_first(Projectile_Queue* queue)
{
    Projectile* to_return = pop_front(queue);

    push_back(queue, to_return);

    return to_return;
}