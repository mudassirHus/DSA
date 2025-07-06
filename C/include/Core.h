#ifndef M_CORE_C
#define M_CORE_C
#include <stdlib.h>
typedef void* (*MallocFunc)(size_t size);
extern MallocFunc Malloc;
#endif