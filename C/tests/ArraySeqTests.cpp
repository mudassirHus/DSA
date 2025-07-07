#include <CppUTest/TestHarness.h>
extern "C"
{
#include "../include/ArraySeq.h"
#include "../include/Core.h"
}
const size_t CorrectAllocationSize = 100;
static size_t DoubleIndex(size_t i) { return i * 2; }

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
        seq = Build(CorrectAllocationSize);
    }
    void teardown() 
    { 
        FreeSeq(&seq);
    }
    // Helper
    void AssertAllocationSuccess(size_t size)
    {
        CHECK_TRUE(NULL != seq);
        CHECK_TRUE(NULL != seq->Sequence);
        LONGS_EQUAL(size, seq->Length);
    }
};
// clang-format on
TEST(ArraySequence_SuccessCases_Build, Allocates_When_Size_Is_Valid)
{
    AssertAllocationSuccess(100);
}

// clang-format off
TEST_GROUP(ArraySequence_SuccessCases_Free)
{
    ArraySeq* seq = NULL;
    void setup()
    {
        seq = Build(CorrectAllocationSize);
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
    seq = NULL;
    AssertAllocationFreeSuccess(seq);
}
TEST(ArraySequence_SuccessCases_Free, Free) { AssertAllocationFreeSuccess(seq); }

// clang-format off
TEST_GROUP(ArraySequence_FailedCases_GetAt)
{
    ArraySeq* seq = NULL;
    void setup()
    {
        seq = Build(CorrectAllocationSize);
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
        seq = Build(CorrectAllocationSize);
    }
    void teardown() 
    {
        FreeSeq(&seq); 
    }
    void AssertGetAt(size_t index, int expectedValue)
    {
        int actualValue = -100;
        CHECK_TRUE(GetAt(index, seq, &actualValue));
        LONGS_EQUAL(expectedValue, actualValue);
    }
};
// clang-format on
TEST(ArraySequence_SuccessCases_GetAt, ShouldReturnZero_After_Allocation) { AssertGetAt(4, 0); }
// clang-format off
TEST_GROUP(ArraySequence_FailureCases_SetAt)
{
    ArraySeq* seq = NULL;
    void setup()
    {
        seq = Build(CorrectAllocationSize);
    }
    void teardown() 
    {
        FreeSeq(&seq); 
    }
    void AssertSetAtFailure(size_t index, ArraySeq* seq, int expectedValue)
    {
        CHECK_FALSE(SetAt(index, seq, expectedValue));
    }
};
// clang-format on
TEST(ArraySequence_FailureCases_SetAt, ShouldReturnNull_For_Failed_Allocation)
{
    AssertSetAtFailure(4, NULL, 65);
}
TEST(ArraySequence_FailureCases_SetAt, ShouldReturnNull_For_Out_Of_Bound)
{
    AssertSetAtFailure(99000, seq, 95);
}
// clang-format off
TEST_GROUP(ArraySequence_SuccessCases_SetAt)
{
    ArraySeq* seq = NULL;
    void setup()
    {
        seq = Build(CorrectAllocationSize);
    }
    void teardown() 
    {
        FreeSeq(&seq); 
    }
    void AssertSetAtSucess(size_t index, int expectedValue)
    {
        CHECK_TRUE(SetAt(index, seq, expectedValue));
        int actualValue = -1;
        GetAt(index, seq, &actualValue);
        LONGS_EQUAL(expectedValue, actualValue);
    }
    void FillSequenceWith(size_t (*formula)(size_t))
    {
        for(size_t i=0;i<seq->Length;i++) SetAt(i,seq, formula(i));
    }
    void CheckSequenceWith(size_t (*formula)(size_t))
    {
        for(size_t i=0;i<seq->Length;i++)
        {
            int actual = -1;
            CHECK_TRUE(GetAt(i, seq, &actual));
            LONGS_EQUAL(formula(i), actual);
        }
    }
};
// clang-format on
TEST(ArraySequence_SuccessCases_SetAt, ShouldReturnExpectedValue_For_In_Bound)
{
    AssertSetAtSucess(5, 56);
}
TEST(ArraySequence_SuccessCases_SetAt, ShouldReturnExpectedValues_For_All_Set)
{
    FillSequenceWith(DoubleIndex);
    CheckSequenceWith(DoubleIndex);
}
TEST_GROUP(ArraySequence_FailureCases_InsertAt)
{
    ArraySeq* seq = NULL;
    void setup() { seq = Build(CorrectAllocationSize); }
    void teardown() { FreeSeq(&seq); }

    void AssertInsertAtFailureForNull(size_t index, int expectedValue)
    {
        CHECK_FALSE(InsertAt(index, NULL, expectedValue));
    }
    void AssertInsertAtFailureNullPointer(size_t index, int expectedValue)
    {
        ArraySeq* NullPointer = NULL;
        CHECK_FALSE(InsertAt(index, &NullPointer, expectedValue));
    }
    void AssertInsertAtFailureForOutOfBound(size_t outOfBoundIndex, int expectedValue)
    {
        CHECK_FALSE(InsertAt(outOfBoundIndex, &seq, expectedValue));
    }
};
TEST(ArraySequence_FailureCases_InsertAt, ShouldReturnNull_When_Failed_Allocation)
{
    AssertInsertAtFailureForNull(12, 120);
    AssertInsertAtFailureNullPointer(12, 120);
}
TEST(ArraySequence_FailureCases_InsertAt, ShouldReturnNull_When_Out_Of_Bound)
{
    AssertInsertAtFailureForOutOfBound(99000, 65);
}

// clang-format off
TEST_GROUP(ArraySequence_SuccessCases_InsertAt)
{
    ArraySeq* seq = NULL;
    void setup() 
    { 
        seq = Build(CorrectAllocationSize); 
    }
    void teardown() 
    { 
        FreeSeq(&seq); 
    }
    void FillSequenceWith(int actualSequence[], size_t size)
    {
        if (seq->Length < size) CHECK(1 == 0);

        for (size_t i = 0; i < size; i++)
        {
            SetAt(i, seq, actualSequence[i]);
        }
    }
    void CheckSequenecWith(int expectedSequence[], size_t size)
    {
        if (seq->Length < size) CHECK(1 == 0);
        
        for (size_t i = 0; i < size; i++)
        {
            SetAt(i, seq, expectedSequence[i]);
        }
    }
};
// clang-format on
TEST(ArraySequence_SuccessCases_InsertAt, ShouldReturnExpectedValues_When_Inserted_In_Middle)
{
    int actual[] = {10, 20, 30, 40, 50};
    int expected[] = {10, 20, 999, 30, 40, 50};
    FillSequenceWith(actual, 5);

    CHECK_TRUE(InsertAt(2, &seq, 999));
    
    CheckSequenecWith(expected, 6);
}
TEST(ArraySequence_SuccessCases_InsertAt, ShouldReturnExpectedValues_When_Inserted_In_First)
{
    int actual[] = {10, 20, 30, 40, 50};
    int expected[] = {999, 10, 20, 30, 40, 50};
    FillSequenceWith(actual, 5);

    CHECK_TRUE(InsertAt(0, &seq, 999));
    
    CheckSequenecWith(expected, 6);
}
TEST(ArraySequence_SuccessCases_InsertAt, ShouldReturnExpectedValues_When_Inserted_In_Last)
{
    int actual[] = {10, 20, 30, 40, 50};
    int expected[] = {10, 20, 30, 40, 999, 50};
    FillSequenceWith(actual, 5);

    CHECK_TRUE(InsertAt(4, &seq, 999));
    
    CheckSequenecWith(expected, 6);
}
// clang-format off
TEST_GROUP(ArraySequence_FailureCases_RemoveAt)
{
    ArraySeq* seq = NULL;
    void setup() 
    { 
        seq = Build(CorrectAllocationSize); 
    }
    void teardown() 
    { 
        FreeSeq(&seq); 
    }
    void AssertRemoveAtFailureForNull(size_t index)
    {
        CHECK_FALSE(RemoveAt(index, NULL));
    }
    void AssertRemoveAtFailureNullPointer(size_t index)
    {
        ArraySeq* NullPointer = NULL;
        CHECK_FALSE(RemoveAt(index, &NullPointer));
    }
    void AssertRemoveAtFailureForOutOfBound(size_t outOfBoundIndex)
    {
        CHECK_FALSE(RemoveAt(outOfBoundIndex, &seq));
    }
};
// clang-format on
TEST(ArraySequence_FailureCases_RemoveAt, ShouldReturnNull_When_Failed_Allocation)
{
    AssertRemoveAtFailureForNull(12);
    AssertRemoveAtFailureNullPointer(16);
    
}
TEST(ArraySequence_FailureCases_RemoveAt, ShouldReturnNull_When_Out_Of_Bound) 
{
    AssertRemoveAtFailureForOutOfBound(9900);
}
// clang-format off
TEST_GROUP(ArraySequence_SuccessCases_RemoveAt)
{
    ArraySeq* seq = NULL;
    void setup() 
    { 
        seq = Build(CorrectAllocationSize); 
    }
    void teardown() 
    { 
        FreeSeq(&seq); 
    }
    void FillSequenceWith(int actualSequence[], size_t size)
    {
        if (seq->Length < size) CHECK(1 == 0);

        for (size_t i = 0; i < size; i++)
        {
            SetAt(i, seq, actualSequence[i]);
        }
    }
    void CheckSequenecWith(int expectedSequence[], size_t size)
    {
        if (seq->Length < size) CHECK(1 == 0);
        
        for (size_t i = 0; i < size; i++)
        {
            SetAt(i, seq, expectedSequence[i]);
        }
    }
};
// clang-format on
TEST(ArraySequence_SuccessCases_RemoveAt, ShouldReturnExpectedValues_When_Removed_In_Middle)
{
    int actual[] = {10, 20, 30, 40, 50};
    int expected[] = {10, 20, 40, 50};
    FillSequenceWith(actual, 5);

    CHECK_TRUE(RemoveAt(2, &seq));
    
    CheckSequenecWith(expected, 4);
}
TEST(ArraySequence_SuccessCases_RemoveAt, ShouldReturnExpectedValues_When_Removed_In_First)
{
    int actual[] = {10, 20, 30, 40, 50};
    int expected[] = {20, 30, 40, 50};
    FillSequenceWith(actual, 5);

    CHECK_TRUE(RemoveAt(0, &seq));
    
    CheckSequenecWith(expected, 4);
}
TEST(ArraySequence_SuccessCases_RemoveAt, ShouldReturnExpectedValues_When_Removed_In_Last)
{
    int actual[] = {10, 20, 30, 40, 50};
    int expected[] = {10, 20, 30, 40};
    FillSequenceWith(actual, 5);

    CHECK_TRUE(RemoveAt(4, &seq));
    
    CheckSequenecWith(expected, 4);
}

// clang-format off
TEST_GROUP(ArraySequence_FailureCases_InsertFirst)
{
    ArraySeq* seq = NULL;
    void setup() 
    { 
        seq = Build(CorrectAllocationSize); 
    }
    void teardown() 
    { 
        FreeSeq(&seq); 
    }
    void AssertInsertFirstFailureForNull(int expectedValue)
    {
        CHECK_FALSE(InsertFirst(NULL, expectedValue));
    }
    void AssertInsertFirstFailureNullPointer(int expectedValue)
    {
        ArraySeq* NullPointer = NULL;
        CHECK_FALSE(InsertFirst(&NullPointer, expectedValue));
    }
};
// clang-format on
TEST(ArraySequence_FailureCases_InsertFirst, ShouldReturnNull_When_Failed_Allocation)
{
    AssertInsertFirstFailureForNull(120);
    AssertInsertFirstFailureNullPointer(120);
}

// clang-format off
TEST_GROUP(ArraySequence_SuccessCases_InsertFirst)
{
    ArraySeq* seq = NULL;
    void setup() 
    { 
        seq = Build(CorrectAllocationSize); 
    }
    void teardown() 
    { 
        FreeSeq(&seq); 
    }
    void FillSequenceWith(int actualSequence[], size_t size)
    {
        if (seq->Length < size) CHECK(1 == 0);

        for (size_t i = 0; i < size; i++)
        {
            SetAt(i, seq, actualSequence[i]);
        }
    }
    void CheckSequenecWith(int expectedSequence[], size_t size)
    {
        if (seq->Length < size) CHECK(1 == 0);
        
        for (size_t i = 0; i < size; i++)
        {
            SetAt(i, seq, expectedSequence[i]);
        }
    }
};
// clang-format on
TEST(ArraySequence_SuccessCases_InsertFirst, ShouldReturnExpectedValues_When_Inserted_At_First)
{
    int actual[] = {10, 20, 30, 40, 50};
    int expected[] = {999, 10, 20, 30, 40, 50};
    FillSequenceWith(actual, 5);

    CHECK_TRUE(InsertFirst(&seq, 999));
    
    CheckSequenecWith(expected, 6);
}

// clang-format off
TEST_GROUP(ArraySequence_FailureCases_InsertLast)
{
    ArraySeq* seq = NULL;
    void setup() 
    { 
        seq = Build(CorrectAllocationSize); 
    }
    void teardown() 
    { 
        FreeSeq(&seq); 
    }
    void AssertInsertLastFailureForNull(int expectedValue)
    {
        CHECK_FALSE(InsertLast(NULL, expectedValue));
    }
    void AssertInsertLastFailureNullPointer(int expectedValue)
    {
        ArraySeq* NullPointer = NULL;
        CHECK_FALSE(InsertLast(&NullPointer, expectedValue));
    }
};
// clang-format on
TEST(ArraySequence_FailureCases_InsertLast, ShouldReturnNull_When_Failed_Allocation)
{
    AssertInsertLastFailureForNull(120);
    AssertInsertLastFailureNullPointer(120);
}

// clang-format off
TEST_GROUP(ArraySequence_SuccessCases_InsertLast)
{
    ArraySeq* seq = NULL;
    void setup() 
    { 
        seq = Build(CorrectAllocationSize); 
    }
    void teardown() 
    { 
        FreeSeq(&seq); 
    }
    void FillSequenceWith(int actualSequence[], size_t size)
    {
        if (seq->Length < size) CHECK(1 == 0);

        for (size_t i = 0; i < size; i++)
        {
            SetAt(i, seq, actualSequence[i]);
        }
    }
    void CheckSequenecWith(int expectedSequence[], size_t size)
    {
        if (seq->Length < size) CHECK(1 == 0);
        
        for (size_t i = 0; i < size; i++)
        {
            SetAt(i, seq, expectedSequence[i]);
        }
    }
};
// clang-format on
TEST(ArraySequence_SuccessCases_InsertLast, ShouldReturnExpectedValues_When_Inserted_At_Last)
{
    int actual[] = {10, 20, 30, 40, 50};
    int expected[] = {10, 20, 30, 40, 50, 999};
    FillSequenceWith(actual, 5);

    CHECK_TRUE(InsertLast(&seq, 999));
    
    CheckSequenecWith(expected, 6);
}

// clang-format off
TEST_GROUP(ArraySequence_FailureCases_RemoveFirst)
{
    ArraySeq* seq = NULL;
    void setup() 
    { 
        seq = Build(CorrectAllocationSize); 
    }
    void teardown() 
    { 
        FreeSeq(&seq); 
    }
    void AssertRemoveFirstFailureForNull()
    {
        CHECK_FALSE(RemoveFirst(NULL));
    }
    void AssertRemoveFirstFailureNullPointer()
    {
        ArraySeq* NullPointer = NULL;
        CHECK_FALSE(RemoveFirst(&NullPointer));
    }
};
// clang-format on
TEST(ArraySequence_FailureCases_RemoveFirst, ShouldReturnNull_When_Failed_Allocation)
{
    AssertRemoveFirstFailureForNull();
    AssertRemoveFirstFailureNullPointer();
}

// clang-format off
TEST_GROUP(ArraySequence_SuccessCases_RemoveFirst)
{
    ArraySeq* seq = NULL;
    void setup() 
    { 
        seq = Build(CorrectAllocationSize); 
    }
    void teardown() 
    { 
        FreeSeq(&seq); 
    }
    void FillSequenceWith(int actualSequence[], size_t size)
    {
        if (seq->Length < size) CHECK(1 == 0);

        for (size_t i = 0; i < size; i++)
        {
            SetAt(i, seq, actualSequence[i]);
        }
    }
    void CheckSequenecWith(int expectedSequence[], size_t size)
    {
        if (seq->Length < size) CHECK(1 == 0);
        
        for (size_t i = 0; i < size; i++)
        {
            SetAt(i, seq, expectedSequence[i]);
        }
    }
};
// clang-format on
TEST(ArraySequence_SuccessCases_RemoveFirst, ShouldReturnExpectedValues_When_Removed_First)
{
    int actual[] = {10, 20, 30, 40, 50};
    int expected[] = {20, 30, 40, 50};
    FillSequenceWith(actual, 5);

    CHECK_TRUE(RemoveFirst(&seq));
    
    CheckSequenecWith(expected, 4);
}

// clang-format off
TEST_GROUP(ArraySequence_FailureCases_RemoveLast)
{
    ArraySeq* seq = NULL;
    void setup() 
    { 
        seq = Build(CorrectAllocationSize); 
    }
    void teardown() 
    { 
        FreeSeq(&seq); 
    }
    void AssertRemoveLastFailureForNull()
    {
        CHECK_FALSE(RemoveLast(NULL));
    }
    void AssertRemoveLastFailureNullPointer()
    {
        ArraySeq* NullPointer = NULL;
        CHECK_FALSE(RemoveLast(&NullPointer));
    }
};
// clang-format on
TEST(ArraySequence_FailureCases_RemoveLast, ShouldReturnNull_When_Failed_Allocation)
{
    AssertRemoveLastFailureForNull();
    AssertRemoveLastFailureNullPointer();
}

// clang-format off
TEST_GROUP(ArraySequence_SuccessCases_RemoveLast)
{
    ArraySeq* seq = NULL;
    void setup() 
    { 
        seq = Build(CorrectAllocationSize); 
    }
    void teardown() 
    { 
        FreeSeq(&seq); 
    }
    void FillSequenceWith(int actualSequence[], size_t size)
    {
        if (seq->Length < size) CHECK(1 == 0);

        for (size_t i = 0; i < size; i++)
        {
            SetAt(i, seq, actualSequence[i]);
        }
    }
    void CheckSequenecWith(int expectedSequence[], size_t size)
    {
        if (seq->Length < size) CHECK(1 == 0);
        
        for (size_t i = 0; i < size; i++)
        {
            SetAt(i, seq, expectedSequence[i]);
        }
    }
};
// clang-format on
TEST(ArraySequence_SuccessCases_RemoveLast, ShouldReturnExpectedValues_When_Removed_Last)
{
    int actual[] = {10, 20, 30, 40, 50};
    int expected[] = {10, 20, 30, 40};
    FillSequenceWith(actual, 5);

    CHECK_TRUE(RemoveLast(&seq));
    
    CheckSequenecWith(expected, 4);
}