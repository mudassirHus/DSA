#include "../include/MallocTestsHelper.h"
#include "../include/Malloc.h"

static unsigned int CallCount = 0;
static unsigned int MallocCount = 0;
static MallocFunc OriginalMalloc = NULL;

static void* MockMalloc(int size)
{
    MallocCount++;
    if (CallCount == MallocCount) return NULL;
    
    // From here original Implementation of Malloc()
    if (size > 0) return calloc(1, (size_t)size);
    return NULL;
}
void SetupCoreMock(void)
{
    OriginalMalloc = Malloc;
    Malloc = MockMalloc;
}
void ResetCoreMock(void)
{
    CallCount = 0;
    MallocCount = 0;
    Malloc = OriginalMalloc;
    OriginalMalloc = NULL;
}
void FailMallocAtCount(unsigned int callCount) { CallCount = callCount; }
