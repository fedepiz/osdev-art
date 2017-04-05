#ifndef UTIL_HEAP_COMMON_H
#define UTIL_HEAP_COMMON_H
#include <stdint.h>
#include <stddef.h>
namespace util {
    namespace heap_common {
#define HEAP_HEADER_MAGIC 0xFEDE

//Represents a memory block
struct heapBlockHeader {
    uint16_t magic; //Should be the constant 0xFEDE, used for consistency checks
    size_t size; //This is the size of the managed memory region, WITHOUT THE HEADER
    heapBlockHeader* nextBlock;
    bool isFree;
};

struct heapSize {
    size_t free;
    size_t allocated;
};

heapBlockHeader* makeMemoryBlock(uint8_t* startAddress, size_t totalSize);
heapBlockHeader* findBlock(heapBlockHeader* head, size_t minSize);
uint8_t* allocate(heapBlockHeader* master, size_t splitSize);
bool tryMergeBlockWithSuccessor(heapBlockHeader* first);
void debugBlock(heapBlockHeader* header);
void debugBlockChain(heapBlockHeader* header);
};};
#endif