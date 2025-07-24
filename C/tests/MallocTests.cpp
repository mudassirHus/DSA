#include <CppUTest/TestHarness.h>
extern "C"
{
#include "../include/Malloc.h"
#include "../include/MallocTestsHelper.h"
}

TEST_GROUP(CustomMalloc_DefaultBehaviour) { void* AllocatedMemory = NULL; };
TEST(CustomMalloc_DefaultBehaviour, AllocationSucceeds_WhenSizeIsValid)
{
    AllocatedMemory = Malloc(TYPICAL_ALLOCATION_SIZE);
    CHECK(NULL != AllocatedMemory);
    free(AllocatedMemory);
}
TEST(CustomMalloc_DefaultBehaviour, AllocationFails_WhenSizeIsZeroBytes)
{
    CHECK(NULL == Malloc(ZERO_BYTES));
}
TEST(CustomMalloc_DefaultBehaviour, AllocationFails_WhenSizeIsNegativeBytes)
{
    CHECK(NULL == Malloc(NEGATIVE_BYTES));
}
TEST_GROUP(CustomMalloc_SimulatedFailure)
{
    void* AllocatedMemory = NULL;
    void setup() { SetupCoreMock(); }
    void teardown() { ResetCoreMock(); }
};
TEST(CustomMalloc_SimulatedFailure, AllocationFails_WhenMemoryIsNotAvailable)
{
    FailMallocAtCount(1);
    AllocatedMemory = Malloc(TYPICAL_ALLOCATION_SIZE);
    CHECK(NULL == AllocatedMemory);
}
