#include "../include/Malloc.h"
static void* MallocDefination(int size)
{
    if (size > 0) return calloc(1, (size_t)size);
    return NULL;
}
MallocFunc Malloc = MallocDefination;