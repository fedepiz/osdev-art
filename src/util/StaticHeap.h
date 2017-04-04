#ifndef UTIL_STATICHEAP
#define UTIL_STATICHEAP
#include <stdint.h>
#include <stddef.h>

#define HEAP_HEADER_MAGIC 0xFEDE

//Represents a memory block
struct heapBlockHeader {
    uint16_t magic; //Should be the constant 0xFEDE, used for consistency checks
    size_t size; //This is the size of the managed memory region, WITHOUT THE HEADER
    heapBlockHeader* nextBlock;
    bool isFree;
};

class StaticHeap {
    private:
    uint8_t* managedRegion;
    heapBlockHeader* headBlock;
    bool isStrict;
    
    heapBlockHeader* findFreeBlock();
    public:
    StaticHeap(uint8_t* managedRegion, size_t regionSize);    
    void* malloc(size_t count);
    void free(void* ptr);
    void debug();
    void setStrict(bool state);
};

extern StaticHeap* k_heap;
void k_heap_initialize(void);
#endif