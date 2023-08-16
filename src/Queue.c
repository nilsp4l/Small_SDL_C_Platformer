#include "../Headers/Queue.h"




Projectile* serve_first(Projectile_Queue* queue)
{
    if(!queue)
    {
        return NULL;
    }

    Projectile to_copy = queue->queue[0];
    // the queue will only be changed if the first element is ready otherwise the functionality of the queue wouldn't be the intended
    if(!to_copy.ready)
    {
        return NULL;
    }

    for(size_t i = 0; i < queue->queue_size - 1; ++i)
    {
        queue->queue[i] = queue->queue[i + 1];   
    }
    
    queue->queue[queue->queue_size - 1] = to_copy;

    return &queue->queue[queue->queue_size - 1];
}