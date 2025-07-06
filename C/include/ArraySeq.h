#ifndef ARRAY_SEQ
#define ARRAY_SEQ
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
typedef struct ArraySeqStruct
{
    int* Sequence;
    size_t Length;
} ArraySeq;
ArraySeq* Build(size_t size);
void FreeSeq(ArraySeq** seq);
bool GetAt(size_t index, ArraySeq* seq, int* out);
bool SetAt(size_t index, ArraySeq* seq, int value);
bool InsertAt(size_t index, ArraySeq** seq, int value);
bool InsertFirst(ArraySeq** seq, int value);
bool InsertLast(ArraySeq** seq, int value);
bool RemoveAt(size_t index, ArraySeq** seq);
bool RemoveFirst(ArraySeq** seq);
bool RemoveLast(ArraySeq** seq);
#endif