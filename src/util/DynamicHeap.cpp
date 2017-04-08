#include <util/DynamicHeap.h>
#include <util/heap_common.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <util/text.h>

namespace util {
    using heap_common::heapBlockHeader;
    using heap_common::heapSize;
    using heap_common::makeMemoryBlock;
    using heap_common::tryMergeBlockWithSuccessor;
    using heap_common::debugBlockChain;
    using heap_common::blockAt;

    using kstd::log;
    using util::logf;
    using util::panicf;
    using kstd::itoa;

    void* watched_ptr = nullptr;

    void DynamicHeap::initialize(memory::page_allocator* page_allocator) {
        this->is_chatty = false;
        this->page_allocator = page_allocator;
        //Allocate the first page
        int first_page = this->page_allocator->allocate();
        uint8_t* first_page_address = (uint8_t*)memory::page_index_to_address(first_page);
        this->memory_base = first_page_address;
        //Implant the first block
        this->head_block = makeMemoryBlock(this->memory_base, memory::BIG_PAGE_SIZE);
    }

    heapBlockHeader* DynamicHeap::last_header() {
        heapBlockHeader* ptr = this->head_block;
        while(ptr->nextBlock != nullptr) {
            ptr = ptr->nextBlock;
        }
        return ptr;
    }


    //Asks the allocator for a new page, attempting to merge it into the last page if possible
    bool DynamicHeap::grow_page() {
        //Asks the allocator for a new page
        int new_page = this->page_allocator->allocate();
        if(new_page == -1) return false;

        //find the last block
        heapBlockHeader* lastBlock = this->last_header();
        //if the last block is free, we can simply increase it's size and
        //that's that
        if(lastBlock->isFree) {
            lastBlock->size += memory::BIG_PAGE_SIZE;
        } else {
            //Last block is not free, make a new block and add it at the end
            uint8_t* page_address = (uint8_t*)memory::page_index_to_address(new_page);
            heapBlockHeader* newBlock = makeMemoryBlock(page_address, memory::BIG_PAGE_SIZE);
            lastBlock->nextBlock = newBlock;
        }
        return true;
    }

    //Grow the heap enough to fit count bytes
    bool DynamicHeap::grow(size_t count) {
        //The actual size we need to satisfy is the requested one plust the size of a new header (as potentially
        //we might need to add a new header)
        size_t totalSize = count + sizeof(heapBlockHeader);
        int num_pages = totalSize/memory::BIG_PAGE_SIZE;
        //If totalSize is not a precise multiple, we must bump it up
        if(totalSize % memory::BIG_PAGE_SIZE != 0) num_pages++;
        //Grow the heap
        for(int i = 0; i < num_pages; i++) {
            bool success = this->grow_page();
            //Abort if failed
            if(!success) return success;
        }
        return true;
    }

    void logHeader(heapBlockHeader* header) {
        uint32_t dataAddress = ((uint32_t)header) + sizeof(heapBlockHeader);
        logf("Data address: %x, size %d, is free %b\n", dataAddress, header->size, header->isFree);
    }

    void* DynamicHeap::malloc(size_t count) {
        //See if there is a large enough block
        heapBlockHeader* fittingHeader = heap_common::findFreeBlock(this->head_block, count);
        if(fittingHeader != nullptr) {
            if(is_chatty) {
                logf("Allocating %d from memory block ", count);
                logHeader(fittingHeader);
            }
            //The block is large enough, proceed with allocation
            return allocate(fittingHeader, count);
        }
        //Attempt to grow the heap sufficiently
        if(!this->grow(count)) {
            //If we failed, return a null pointer
            return nullptr;
        } else {
            //We succeded, retry allocation
            fittingHeader = heap_common::findFreeBlock(this->head_block, count);
            return allocate(fittingHeader, count);
        }
    }
    
    heap_common::heapBlockHeader* DynamicHeap::_free(void* ptr) {
        if(ptr == watched_ptr) {
            log("Attempting to free watched pointer\n");
        }
        heapBlockHeader* header = heap_common::blockAt(ptr);
        if(this->is_chatty) {
            log("Freeing memory block: ");
            logHeader(header);
        }
        if(header == nullptr) {
            log(kstd::itoa((uint32_t)ptr,16).str);
            this->debug();
            panic("Not a valid header block in free");
        }
        if(header->isFree) {
            panic("Attempting to free an unallocated block");
        }
        header->isFree = true;
        heap_common::tryMergeBlockWithSuccessor(header);
        return header;
    }

    //For now we have a restricted form of free: it will release the block and try to merge it,
    //but it will not tell the OS the memory is out again.
    void DynamicHeap::free(void* ptr) {
        this->_free(ptr);
    }

    void DynamicHeap::cfree(void* ptr) {
        heapBlockHeader* header = this->_free(ptr);
        kstd::memset(ptr, 0, header->size);
    }

    void DynamicHeap_initialize(DynamicHeap* heap, memory::page_allocator* allocator) {
        heap->initialize(allocator);
    }

    void DynamicHeap::debug() {
        debugBlockChain(this->head_block);
    }

    void DynamicHeap::chatty_mode(bool mode) {
        this->is_chatty = mode;
    }
};