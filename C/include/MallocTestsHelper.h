#ifndef MALLOC_TEST_HELPER
#define MALLOC_TEST_HELPER
#define TYPICAL_ALLOCATION_SIZE 100
#define ZERO_BYTES 0
#define NEGATIVE_BYTES -100
void SetupCoreMock(void);
void ResetCoreMock(void);
void FailMallocAtCount(unsigned int callCount);
#endif