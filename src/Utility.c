#include "../Headers/Utility.h"

void zero_array(char* array, size_t size)
{
    for(size_t i = 0; i < size; ++i)
    {
        *(array + i) = 0;
    }
}