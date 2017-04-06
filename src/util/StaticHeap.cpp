#include <util/StaticHeap.h>
#include <kstdio.h>

namespace util {
using kstd::log;

using heap_common::heapBlockHeader;
using heap_common::heapSize;
using heap_common::makeMemoryBlock;
using heap_common::tryMergeBlockWithSuccessor;
using heap_common::debugBlockChain;

StaticHeap::StaticHeap(uint8_t* memPtr, size_t size) {
    this->managedRegion = memPtr;
    this->headBlock = makeMemoryBlock(this->managedRegion, size);
}

heapBlockHeader* StaticHeap::findFreeBlock() {
    heapBlockHeader* ptr = this->headBlock;
    while(ptr != nullptr && !ptr->isFree) {
        ptr = ptr->nextBlock;
    }
    if(ptr == nullptr && this->isStrict) {
        panic("Cannot find free block");
    }
    return ptr;
}

void* StaticHeap::malloc(size_t count) {
    heapBlockHeader* freeBlock = findFreeBlock();
    if(freeBlock == nullptr)
        return nullptr;
    void* memPtr = allocate(freeBlock, count);
    if(memPtr == nullptr && this->isStrict) {
        panic("Failed allocating block");
    }
    return memPtr;
}

void StaticHeap::free(void* genPtr) {
    uint8_t* ptr = (uint8_t*) genPtr;
    heapBlockHeader* header = (heapBlockHeader*)(ptr - sizeof(heapBlockHeader));
    if(header->magic != HEAP_HEADER_MAGIC) {
        panic("Magic number mismatch, not a valid heap block header");
    }
    if(header->isFree) {
        panic("Attempting double free");
    }
    header->isFree = true;
    tryMergeBlockWithSuccessor(header);
}

heapSize StaticHeap::getSize() {
    heapSize size;
    size.free = 0;
    size.allocated = 0;
    heapBlockHeader* header = this->headBlock;
    while(header != nullptr) {
        if(header->isFree){
            size.free += header->size;
        } else {
            size.allocated += header->size;
        }
        header = header->nextBlock;
    }
    return size;
}



void StaticHeap::setStrict(bool state) {
    this->isStrict = state;
}

void StaticHeap::debug() {
    debugBlockChain(this->headBlock);
}

//These are not functions really, just pieces of data,
//but we use them as address markers.
extern "C" void heap_top();
extern "C" void heap_bottom();
};