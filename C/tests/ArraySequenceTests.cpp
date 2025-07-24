#include <CppUTest/TestHarness.h>
extern "C"
{
#include "../include/ArraySequence.h"
#include "../include/Malloc.h"
#include "../include/MallocTestsHelper.h"
}
#define HUNDRED_SIZE 100
#define MIDDLE_INDEX 49
#define NEGATIVE_FIFTY -50
#define ZERO 0

TEST_GROUP(BuildSequence_DefaultBehaviour) { ArraySequence* Sequence = NULL; };
TEST(BuildSequence_DefaultBehaviour, Success_WhenSizeIsValid)
{
    Sequence = BuildSequence(TYPICAL_ALLOCATION_SIZE);
    CHECK_TRUE(NULL != Sequence);
    LONGS_EQUAL(TYPICAL_ALLOCATION_SIZE, Sequence->Length);
    free(Sequence);
}
TEST(BuildSequence_DefaultBehaviour, Failure_WhenSizeNotValid)
{
    CHECK_TRUE(NULL == BuildSequence(ZERO_BYTES));
    CHECK_TRUE(NULL == BuildSequence(NEGATIVE_BYTES));
}
TEST_GROUP(BuildSequence_SimulatedFailures)
{
    ArraySequence* Sequence = NULL;
    void setup() { SetupCoreMock(); }
    void teardown() { ResetCoreMock(); }
};
TEST(BuildSequence_SimulatedFailures, Failure_WhenMemoryIsNotAvailable)
{
    FailMallocAtCount(1);
    CHECK_TRUE(NULL == BuildSequence(TYPICAL_ALLOCATION_SIZE));
}
TEST_GROUP(GetAt_DefaultBehaviour)
{
    ArraySequence* Sequence = NULL;
    void setup() { SetupCoreMock(); }
    void teardown() { ResetCoreMock(); }
};
TEST(GetAt_DefaultBehaviour, ReturnsZero_AfterSequenceIsAllocated)
{
    ArraySequence* Sequence = BuildSequence(HUNDRED_SIZE);
    int actual = NEGATIVE_FIFTY;
    CHECK_TRUE(GetAt(MIDDLE_INDEX, Sequence, &actual));
    LONGS_EQUAL(ZERO, actual);
    free(Sequence);
}
