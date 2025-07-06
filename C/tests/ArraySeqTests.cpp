#include <CppUTest/TestHarness.h>
extern "C"
{
#include "../include/ArraySeq.h"
#include "../include/Core.h"
}

static MallocFunc OriginalMalloc = Malloc;
static int FailCountAt = 0;
static int MallocCount = 0;
static void* MallocMock(size_t size)
{
    MallocCount++;
    if (FailCountAt == MallocCount) return NULL;
    if (size > 0) return calloc(1, size);
    return NULL;
}

// clang-format off
TEST_GROUP(ArraySequence_FailureCases_Build)
{
    void setup()
    {
        FailCountAt = 0;
        MallocCount = 0;
        Malloc = MallocMock;
    }
    void teardown() 
    { 
        FailCountAt = 0;
        MallocCount = 0;
        Malloc = OriginalMalloc;
    }
    // Helper
    void AssertAllocationFailed(size_t size)
    {
        ArraySeq* ArraySequence = Build(size);
        POINTERS_EQUAL(NULL, ArraySequence);
    }
};
// clang-format on
TEST(ArraySequence_FailureCases_Build, ShouldReturnNull_When_First_AllocationFails)
{
    FailCountAt = 1;
    AssertAllocationFailed(100);
}
TEST(ArraySequence_FailureCases_Build, ShouldReturnNull_When_Second_AllocationFails)
{
    FailCountAt = 2;
    AssertAllocationFailed(100);
}

// clang-format off
TEST_GROUP(ArraySequence_SuccessCases_Build)
{
    ArraySeq* seq = NULL;
    void setup()
    {
        seq = Build(100);
    }
    void teardown() 
    { 
        FreeSeq(&seq);
    }
    // Helper
    void AssertAllocationSuccess(size_t size, ArraySeq* seq)
    {
        CHECK_TRUE(NULL != seq);
        CHECK_TRUE(NULL != seq->Sequence);
        LONGS_EQUAL(size, seq->Length);
    }
};
// clang-format on
TEST(ArraySequence_SuccessCases_Build, Allocates_When_Size_Is_Valid)
{
    AssertAllocationSuccess(100, seq);
}

// clang-format off
TEST_GROUP(ArraySequence_SuccessCases_Free)
{
    ArraySeq* seq = NULL;
    void setup()
    {
        seq = Build(100);
    }
    // Helper
    void AssertAllocationFreeSuccess(ArraySeq* seq)
    {
        FreeSeq(&seq);
        POINTERS_EQUAL(NULL, seq);
    }
};
// clang-format on
TEST(ArraySequence_SuccessCases_Free, Ignores_Null_Parameter)
{
    ArraySeq* NullSequence = NULL;
    AssertAllocationFreeSuccess(NullSequence);
}
TEST(ArraySequence_SuccessCases_Free, Free) { AssertAllocationFreeSuccess(seq); }

// clang-format off
TEST_GROUP(ArraySequence_FailedCases_GetAt)
{
    ArraySeq* seq = NULL;
    void setup()
    {
        seq = Build(10);
    }
    void teardown() 
    {
        FreeSeq(&seq); 
    }
    void AssertGetAt(size_t index, ArraySeq* sequence, int expectedValue)
    {
        int actualValue = -100;
        CHECK_FALSE(GetAt(index, sequence, &actualValue));
        LONGS_EQUAL(expectedValue, actualValue);
    }
};
// clang-format on
TEST(ArraySequence_FailedCases_GetAt, ShouldReturnFalse_For_Failed_Allocation)
{
    AssertGetAt(4, NULL, -1);
}
TEST(ArraySequence_FailedCases_GetAt, ShouldReturnFalse_When_Out_Of_Bound)
{
    AssertGetAt(98000, seq, -1);
}

// clang-format off
TEST_GROUP(ArraySequence_SuccessCases_GetAt)
{
    ArraySeq* seq = NULL;
    void setup()
    {
        seq = Build(10);
    }
    void teardown() 
    {
        FreeSeq(&seq); 
    }
    void AssertGetAt(size_t index, ArraySeq* sequence, int expectedValue)
    {
        int actualValue = -100;
        CHECK_TRUE(GetAt(index, sequence, &actualValue));
        LONGS_EQUAL(expectedValue, actualValue);
    }
};
// clang-format on
TEST(ArraySequence_SuccessCases_GetAt, ShouldReturnZero_After_Allocation)
{
    AssertGetAt(4, seq, 0);
}


/* TODO: Clean up from here
TEST(ArraySeqApi, SetAt_Uninit) { CHECK_FALSE(SetAt(4, NULL, 65)); }
TEST(ArraySeqApi, SetAt_Out_Of_Bound) { CHECK_FALSE(SetAt(90000, NULL, 65)); }
TEST(ArraySeqApi, SetAt_In_Bound) { CHECK_TRUE(SetAt(5, seq, 65)); }
TEST(ArraySeqApi, GetAt_AfterSet)
{
    SetAt(4, seq, 65);
    int actual = 0;
    CHECK_TRUE(GetAt(4, seq, &actual));
    LONGS_EQUAL(65, actual);
}
TEST(ArraySeqApi, GetAt_AfterAllSet)
{
    for (size_t i = 0; i < seq->Length; i++)
    {
        CHECK_TRUE(SetAt(i, seq, i + 1));
    }

    int actual = 0;
    for (size_t i = 0; i < seq->Length; i++)
    {
        CHECK_TRUE(GetAt(i, seq, &actual));
        LONGS_EQUAL(i + 1, actual);
    }
}

TEST(ArraySeqApi, InsertAt_UnInit)
{
    ArraySeq* seq = NULL;
    CHECK_FALSE(InsertAt(9000, NULL, 77));
    CHECK_FALSE(InsertAt(91000, &seq, 77));
}
TEST(ArraySeqApi, InsertAt_Out_Of_Bound) { CHECK_FALSE(InsertAt(9000, &seq, 77)); }
TEST(ArraySeqApi, InsertAt_InBound)
{
    int actualValues[10] = {0, 11, 22, 33, 44, 55, 66, 77, 88, 99};
    for (size_t i = 0; i < seq->Length; i++)
    {
        SetAt(i, seq, actualValues[i]);
    }
    LONGS_EQUAL(10, seq->Length);
    CHECK_TRUE(InsertAt(4, &seq, 919191));
    LONGS_EQUAL(11, seq->Length);

    int expectedValues[11] = {0, 11, 22, 33, 919191, 44, 55, 66, 77, 88, 99};
    for (size_t i = 0; i < seq->Length; i++)
    {
        int actualVal = -1;
        CHECK_TRUE(GetAt(i, seq, &actualVal));
        LONGS_EQUAL(expectedValues[i], actualVal);
    }
}
TEST(ArraySeqApi, RemoveAt_Unit)
{
    ArraySeq* seq = NULL;
    CHECK_FALSE(RemoveAt(9000, NULL));
    CHECK_FALSE(RemoveAt(9900, &seq));
}
TEST(ArraySeqApi, RemoveAt_Out_Of_Bound) { CHECK_FALSE(RemoveAt(9000, &seq)); }
TEST(ArraySeqApi, RemoveAt_InBound)
{
    int actualValues[10] = {0, 11, 22, 33, 44, 55, 66, 77, 88, 99};
    for (size_t i = 0; i < seq->Length; i++)
    {
        SetAt(i, seq, actualValues[i]);
    }
    LONGS_EQUAL(10, seq->Length);
    CHECK_TRUE(RemoveAt(4, &seq));
    LONGS_EQUAL(9, seq->Length);

    int expectedValues[9] = {0, 11, 22, 33, 55, 66, 77, 88, 99};
    for (size_t i = 0; i < seq->Length; i++)
    {
        int actualVal = -1;
        CHECK_TRUE(GetAt(i, seq, &actualVal));
        LONGS_EQUAL(expectedValues[i], actualVal);
    }
}
TEST(ArraySeqApi, InsertAt_First_InBound)
{
    int actualValues[10] = {0, 11, 22, 33, 44, 55, 66, 77, 88, 99};
    for (size_t i = 0; i < seq->Length; i++)
    {
        SetAt(i, seq, actualValues[i]);
    }
    LONGS_EQUAL(10, seq->Length);
    CHECK_TRUE(InsertAt(0, &seq, 919191));
    LONGS_EQUAL(11, seq->Length);

    int expectedValues[11] = {919191, 0, 11, 22, 33, 44, 55, 66, 77, 88, 99};
    for (size_t i = 0; i < seq->Length; i++)
    {
        int actualVal = -1;
        CHECK_TRUE(GetAt(i, seq, &actualVal));
        LONGS_EQUAL(expectedValues[i], actualVal);
    }
}
TEST(ArraySeqApi, InsertLast_Unit)
{
    ArraySeq* seq = NULL;
    CHECK_FALSE(InsertLast(NULL, 99));
    CHECK_FALSE(InsertLast(&seq, 101));
}
TEST(ArraySeqApi, InsertLast)
{
    int actualValues[10] = {0, 11, 22, 33, 44, 55, 66, 77, 88, 99};
    for (size_t i = 0; i < seq->Length; i++)
    {
        SetAt(i, seq, actualValues[i]);
    }
    LONGS_EQUAL(10, seq->Length);
    CHECK_TRUE(InsertLast(&seq, 919191));
    LONGS_EQUAL(11, seq->Length);
    int expectedValues[11] = {0, 11, 22, 33, 44, 55, 66, 77, 88, 99, 919191};
    for (size_t i = 0; i < seq->Length; i++)
    {
        int actualVal = -1;
        CHECK_TRUE(GetAt(i, seq, &actualVal));
        LONGS_EQUAL(expectedValues[i], actualVal);
    }
}
TEST(ArraySeqApi, InsertFirst)
{
    int actualValues[10] = {0, 11, 22, 33, 44, 55, 66, 77, 88, 99};
    for (size_t i = 0; i < seq->Length; i++)
    {
        SetAt(i, seq, actualValues[i]);
    }
    LONGS_EQUAL(10, seq->Length);
    CHECK_TRUE(InsertFirst(&seq, 919191));
    LONGS_EQUAL(11, seq->Length);
    int expectedValues[11] = {919191, 0, 11, 22, 33, 44, 55, 66, 77, 88, 99};
    for (size_t i = 0; i < seq->Length; i++)
    {
        int actualVal = -1;
        CHECK_TRUE(GetAt(i, seq, &actualVal));
        LONGS_EQUAL(expectedValues[i], actualVal);
    }
}
TEST(ArraySeqApi, RemoveFirst)
{
    int actualValues[10] = {0, 11, 22, 33, 44, 55, 66, 77, 88, 99};
    for (size_t i = 0; i < seq->Length; i++)
    {
        SetAt(i, seq, actualValues[i]);
    }
    LONGS_EQUAL(10, seq->Length);
    CHECK_TRUE(RemoveFirst(&seq));
    LONGS_EQUAL(9, seq->Length);

    int expectedValues[9] = {11, 22, 33, 44, 55, 66, 77, 88, 99};
    for (size_t i = 0; i < seq->Length; i++)
    {
        int actualVal = -1;
        CHECK_TRUE(GetAt(i, seq, &actualVal));
        LONGS_EQUAL(expectedValues[i], actualVal);
    }
}
TEST(ArraySeqApi, RemoveLast_Unit)
{
    ArraySeq* seq = NULL;
    CHECK_FALSE(RemoveLast(NULL));
    CHECK_FALSE(RemoveLast(&seq));
}
TEST(ArraySeqApi, RemoveLast)
{
    int actualValues[10] = {0, 11, 22, 33, 44, 55, 66, 77, 88, 99};
    for (size_t i = 0; i < seq->Length; i++)
    {
        SetAt(i, seq, actualValues[i]);
    }
    LONGS_EQUAL(10, seq->Length);
    CHECK_TRUE(RemoveLast(&seq));
    LONGS_EQUAL(9, seq->Length);

    int expectedValues[9] = {0, 11, 22, 33, 44, 55, 66, 77, 88};
    for (size_t i = 0; i < seq->Length; i++)
    {
        int actualVal = -1;
        CHECK_TRUE(GetAt(i, seq, &actualVal));
        LONGS_EQUAL(expectedValues[i], actualVal);
    }
}
    */