#ifndef M_CORE_C
#define M_CORE_C
#include <stdlib.h>
typedef void* (*MallocFunc)(int size);
extern MallocFunc Malloc;
#endif