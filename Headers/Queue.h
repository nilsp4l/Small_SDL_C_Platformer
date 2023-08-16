#include "Projectile.h"


struct projectile_queue
{
    Projectile* queue;
    size_t queue_size;
};

typedef struct projectile_queue Projectile_Queue;


Projectile* serve_first(Projectile_Queue* queue);