#ifndef ARRAY_SEQUENCE
#define ARRAY_SEQUENCE
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
typedef struct ArraySequenceStruct
{
    int* Sequence;
    size_t Length;
} ArraySequence;
ArraySequence* BuildSequence(int size);
void FreeSequence(ArraySequence** seq);
bool GetAt(size_t index, ArraySequence* sequence, int* out);
bool SetAt(size_t index, ArraySequence* seq, int value);
bool InsertAt(size_t index, ArraySequence** seq, int value);
bool InsertFirst(ArraySequence** seq, int value);
bool InsertLast(ArraySequence** seq, int value);
bool RemoveAt(size_t index, ArraySequence** seq);
bool RemoveFirst(ArraySequence** seq);
bool RemoveLast(ArraySequence** seq);
#endif