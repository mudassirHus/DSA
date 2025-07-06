#include "../include/Core.h"
static void* MallocDefination(size_t size)
{
    if (size > 0) return calloc(1, size);
    return NULL;
}
MallocFunc Malloc = MallocDefination;