#include "Projectile.h"


struct projectile_queue
{
    Projectile** queue;
    size_t queue_size;
};

typedef struct projectile_queue Projectile_Queue;

int push_back(Projectile_Queue* queue, Projectile* to_insert);
Projectile* pop_front(Projectile_Queue* queue);
Projectile* serve_first(Projectile_Queue* queue);