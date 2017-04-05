#include <util/heap_common.h>
#include <kernel/kstd.h>

namespace util {
    namespace heap_common {
#define MIN_BLOCK_SIZE (sizeof(heapBlockHeader)*4)
//Use 32-bit alignment for blocks
const int alignment = 4;

using kstd::log;
using kstd::itoa;
//Inserts a memory block, with the header starting at startAddress. Total size
//is the total size of the memory block, INCLUDING THE HEADER
heapBlockHeader* makeMemoryBlock(uint8_t* startAddress, size_t totalSize) {
    heapBlockHeader* header = (heapBlockHeader*)startAddress;
    size_t regionSize = totalSize - sizeof(heapBlockHeader);
    header->magic = HEAP_HEADER_MAGIC;
    header->size = regionSize;
    header->nextBlock = nullptr;
    header->isFree = true;
    return header;
}

heapBlockHeader* findFreeBlock(heapBlockHeader* head, size_t minSize) {
    while(head != nullptr) {
        if(head->isFree && head->size >= minSize) {
            return head;
        }
        head = head->nextBlock;
    }
    //Not found
    return nullptr;
}

heapBlockHeader* blockAt(void* genPtr) {
    uint8_t* ptr = (uint8_t*)genPtr;
    heapBlockHeader* header = (heapBlockHeader*)(ptr - sizeof(heapBlockHeader));
    if(header->magic != HEAP_HEADER_MAGIC) {
        return nullptr;//kstd::panic("Magic number mismatch, not a valid heap block header");
    }
    return header;
}

//Allocates a block of memory, splitting it off a current existing block
uint8_t* allocate(heapBlockHeader* master, size_t splitSize) {
    //Make split-size aligned on a 32-bit bounduary
    if(splitSize % alignment != 0) splitSize += alignment - (splitSize % alignment);
    if(master->size < splitSize) {
        //Not enough memory in block
        return nullptr;
    }
    if(master->size < MIN_BLOCK_SIZE) {
        //The block is already of minimal size, simply mark as allocated and
        //return the pointer to the memory
        master->isFree = false;
        uint8_t* basePtr = (uint8_t*)master;
        return basePtr + sizeof(heapBlockHeader);
    }
    //Block is big enough to split
    uint8_t* basePtr = (uint8_t*)master;
    //advance the base pointer after the size
     uint8_t* newBlockHeaderPtr = basePtr + sizeof(heapBlockHeader) + splitSize;
    //make the new chld block
    heapBlockHeader *newBlock = makeMemoryBlock(newBlockHeaderPtr, master->size - splitSize);
    //Adjust the block links
     newBlock->nextBlock = master->nextBlock;
     master->nextBlock = newBlock;
     //Shrink master and allocate it
     master->size = master->size - (sizeof(heapBlockHeader) + newBlock->size);
      master->isFree = false;
     return basePtr + sizeof(heapBlockHeader);
}

bool tryMergeBlockWithSuccessor(heapBlockHeader* first) {
    if(first == nullptr || 
    first->isFree == false ||
    first->nextBlock == nullptr  || 
    first->nextBlock->isFree == false) {
        return false;
    }
    auto second = first->nextBlock;
    first->size = first->size + second->size + sizeof(heapBlockHeader);
    first->nextBlock = second->nextBlock;
    //Erease magic
    second->magic = 0;
    return true;
}

void debugBlock(heapBlockHeader* header) {
    log("Debugging header block\n");
    if(header == nullptr) {
        log("Null block address\n");
    }
    uint32_t address = (uint32_t)header;
    log("Header address:");
    log(itoa(address,16).str);
    log("\nMagic: ");
    log(itoa(header->magic).str);
    log("\nSize: ");
    log(itoa(header->size).str);
    log("\nFree: ");
    if(header->isFree)
        log("Y\n");
    else
        log("N\n");
}

void debugBlockChain(heapBlockHeader* header) {
    log("Debugging heap block chain\n...\n");
    while(true) {
        debugBlock(header);
        header = header->nextBlock;
        if(header == nullptr) {
            return;
        }
    }
}
};
};