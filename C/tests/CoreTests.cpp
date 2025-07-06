#include <CppUTest/TestHarness.h>
extern "C"
{
#include "../include/Core.h"
}

// Test Constants
const size_t TYPICAL_ALLOCATION_SIZE = 100;
const size_t ZERO_BYTES = 0;

static MallocFunc OriginalMalloc = Malloc;
static bool ShouldMallocFail = 0;

static void* MallocMock(size_t size)
{
    if (ShouldMallocFail) return NULL;
    if (size > 0) return calloc(1, size);
    return NULL;
}

// Helpers
static void AssertAllocationFailed(size_t size) { POINTERS_EQUAL(NULL, Malloc(size)); }
static void AssertAllocationSuccess(size_t size)
{
    void* allocated = Malloc(size);
    CHECK_TRUE(NULL != allocated);
    free(allocated);
}

// clang-format off
TEST_GROUP(CustomMalloc_FailureCases)
{
    void setup() 
    { 
        Malloc = MallocMock;
        ShouldMallocFail = 1;
    }
    void teardown() 
    { 
        Malloc = OriginalMalloc; 
    }
};
// clang-format on

TEST(CustomMalloc_FailureCases, ShouldReturnNull_WhenAllocationFails)
{
    AssertAllocationFailed(TYPICAL_ALLOCATION_SIZE);
}
TEST(CustomMalloc_FailureCases, ShouldReturnNull_WhenRequestingZeroBytes)
{
    AssertAllocationFailed(ZERO_BYTES);
}

// clang-format off
TEST_GROUP(CustomMalloc_SuccessCases)
{
    void setup() 
    {
        Malloc = OriginalMalloc;
    }
};
// clang-format on

TEST(CustomMalloc_SuccessCases, ShouldAllocateMemory_WhenSizeIsValid)
{
    AssertAllocationSuccess(TYPICAL_ALLOCATION_SIZE);
}