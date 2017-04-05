#ifndef UTIL_STATICHEAP
#define UTIL_STATICHEAP
#include <stdint.h>
#include <stddef.h>
#include <util/heap_common.h>

namespace util {
class StaticHeap {
    private:
    uint8_t* managedRegion;
    heap_common::heapBlockHeader* headBlock;
    bool isStrict;
    
    heap_common::heapBlockHeader* findFreeBlock();
    public:
    StaticHeap(uint8_t* managedRegion, size_t regionSize);    
    void* malloc(size_t count);
    void free(void* ptr);
    heap_common::heapSize getSize();
    void debug();
    void setStrict(bool state);
};
};
#endif