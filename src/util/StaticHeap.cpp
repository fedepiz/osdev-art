#include <util/StaticHeap.h>
#include <kernel/std.h>

#define MIN_BLOCK_SIZE (sizeof(heapBlockHeader)*4)

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

//Allocates a block of memory, splitting it off a current existing block
uint8_t* allocate(heapBlockHeader* master, size_t splitSize) {
    if(master->size < splitSize) {
        //Not enough memory in block
        return nullptr;
    }
    if(master->size < MIN_BLOCK_SIZE) {
        //The block is already of minimal size, simply mark as alocated and
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
    k_debug_writestring("Debugging header block\n");
    if(header == nullptr) {
        k_debug_writestring("Null block address\n");
    }
    uint32_t address = (uint32_t)header;
    k_debug_writestring("Header address:");
    k_debug_writestring(itoa(address).str);
    k_debug_writestring("\nMagic: ");
    k_debug_writestring(itoa(header->magic).str);
    k_debug_writestring("\nSize: ");
    k_debug_writestring(itoa(header->size).str);
    k_debug_writestring("\n");
}

void debugBlockChain(heapBlockHeader* header) {
    k_debug_writestring("Debugging heap block chain\n...\n");
    while(true) {
        debugBlock(header);
        header = header->nextBlock;
        if(header == nullptr) {
            return;
        }
    }
}

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
        k_panic("Cannot find free block");
    }
    return ptr;
}

void* StaticHeap::malloc(size_t count) {
    heapBlockHeader* freeBlock = findFreeBlock();
    if(freeBlock == nullptr)
        return nullptr;
    void* memPtr = allocate(freeBlock, count);
    if(memPtr == nullptr && this->isStrict) {
        k_panic("Failed allocating block");
    }
    return memPtr;
}

void StaticHeap::free(void* genPtr) {
    uint8_t* ptr = (uint8_t*) genPtr;
    heapBlockHeader* header = (heapBlockHeader*)(ptr - sizeof(heapBlockHeader));
    if(header->magic != HEAP_HEADER_MAGIC) {
        k_panic("Magic number mismatch, not a valid heap block header");
    }
    if(header->isFree) {
        k_panic("Attempting double free");
    }
    header->isFree = true;
    tryMergeBlockWithSuccessor(header);
}

void StaticHeap::setStrict(bool state) {
    this->isStrict = state;
}

void StaticHeap::debug() {
    debugBlockChain(this->headBlock);
}