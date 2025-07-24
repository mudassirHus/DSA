#include "../include/ArraySequence.h"
#include "../include/Malloc.h"
#include <stdbool.h>

ArraySequence* BuildSequence(int size)
{
    if (size <= 0) return NULL;

    ArraySequence* Sequence = Malloc(sizeof(ArraySequence));
    if (Sequence == NULL) return NULL;

    Sequence->Length = (size_t)size;
    return Sequence;
}
bool GetAt(size_t index, ArraySequence* sequence, int* out)
{
    *out = 0;
    return true;
}
// bool GetAt(size_t index, ArraySequence* seq, int* out)
// {
//     if (seq && index < seq->Length)
//     {
//         *out = seq->Sequence[index];
//         return true;
//     }
//     *out = -1;
//     return false;
// }
// bool SetAt(size_t index, ArraySequence* sequence, int value)
// {
//     return true;
// }
// return NULL;
//     ArraySeq* arraySeq = Malloc(sizeof(ArraySeq));
//     if (arraySeq == NULL) return NULL;

//     arraySeq->Sequence = Malloc(sizeof(int) * size);
//     if (arraySeq->Sequence == NULL)
//     {
//         free(arraySeq);
//         return NULL;
//     }
//     arraySeq->Length = size;
//     return arraySeq;
// void FreeSeq(ArraySeq** seq)
// {
//     if (seq && *seq)
//     {
//         free((*seq)->Sequence);
//         free(*seq);
//         *seq = NULL; // Set caller's pointer to NULL
//     }
// }

// bool SetAt(size_t index, ArraySeq* seq, int value)
// {
//     if (seq && index < seq->Length)
//     {
//         seq->Sequence[index] = value;
//         return true;
//     }
//     return false;
// }
// bool InsertAt(size_t index, ArraySeq** seq, int value)
// {
//     if (seq && *seq && index < (*seq)->Length)
//     {
//         ArraySeq* newSeq = Build((*seq)->Length + 1);
//         if (!newSeq) return false;

//         for (size_t i = 0; i < index; i++)
//         {
//             SetAt(i, newSeq, (*seq)->Sequence[i]);
//         }
//         SetAt(index, newSeq, value);
//         for (size_t i = index + 1; i < newSeq->Length; i++)
//         {
//             SetAt(i, newSeq, (*seq)->Sequence[i - 1]);
//         }
//         FreeSeq(seq);
//         *seq = newSeq;
//         return true;
//     }
//     return false;
// }
// bool RemoveAt(size_t index, ArraySeq** seq)
// {
//     if (seq && *seq && index < (*seq)->Length)
//     {
//         ArraySeq* newSeq = Build((*seq)->Length - 1);
//         if (!newSeq) return false;

//         for (size_t i = 0; i < index; i++)
//         {
//             SetAt(i, newSeq, (*seq)->Sequence[i]);
//         }
//         for (size_t i = index; i < newSeq->Length; i++)
//         {
//             SetAt(i, newSeq, (*seq)->Sequence[i + 1]);
//         }
//         FreeSeq(seq);
//         *seq = newSeq;
//         return true;
//     }
//     return false;
// }
// bool InsertLast(ArraySeq** seq, int value)
// {
//     if (seq && *seq)
//     {
//         ArraySeq* newSeq = Build((*seq)->Length + 1);
//         if (!newSeq) return false;

//         for (size_t i = 0; i < (*seq)->Length; i++)
//         {
//             SetAt(i, newSeq, (*seq)->Sequence[i]);
//         }
//         SetAt(newSeq->Length - 1, newSeq, value);
//         FreeSeq(seq);
//         *seq = newSeq;
//         return true;
//     }
//     return false;
// }
// bool InsertFirst(ArraySeq** seq, int value) { return InsertAt(0, seq, value); }
// bool RemoveFirst(ArraySeq** seq) { return RemoveAt(0, seq); }
// bool RemoveLast(ArraySeq** seq)
// {
//     if (seq && *seq)
//     {
//         return RemoveAt((*seq)->Length - 1, seq);
//     }
//     return false;
// }