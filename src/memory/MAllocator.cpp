#include <memory/MAllocator.h>
#include <kstdlib.h>
#include <util/text.h>
#include <memory/subsystem.h>

namespace memory {
    using util::panicf;
    using util::logf;

    const size_t MALLOCATOR_MIN_BLOCK_SIZE = 32;

    size_t MAllocator::header::totalSize() {
        return this->dataSize;
    }

    char* MAllocator::header::infoPtr() {
        return this->info;
    }

    uint8_t* MAllocator::header::dataPtr() {
        auto this_ptr = (uint8_t*)this;
        return this_ptr + sizeof(header);
    }

    void MAllocator::header::check() {
        if(this->magic != 0xFEDE) {
            panicf("Not a valid block\naddress = %x data = %x, size = %d b, magic = %x, tag = [%s] free = %b\n",
                (uint32_t)this, (uint32_t)this->dataPtr(), this->dataSize, this->magic, this->info, this->free
            );
        }
    }

    void MAllocator::header::carpet_with(uint8_t value) {
        auto ptr = (uint8_t*)this + sizeof(header);
        auto length = this->totalSize();
        kstd::memset(ptr, value, length);
    }

    MAllocator::header* MAllocator::makeBlock(size_t offset, char* infoPtr, size_t dataSize) {
        //Compute the actual buffer
        uint8_t* buffer = this->memory_base + offset;
         //The header goes right at the start
         header* header_ptr = (header*)buffer;

         //Populate the header information
         header_ptr->magic = 0xFEDE;
         header_ptr->dataSize = dataSize;
         header_ptr->free = true;
         //Copy the information into the info region
         if(infoPtr != nullptr) {
             if((kstd::strlen(infoPtr)+1) > MAllocator::HEADER_INFO_SIZE) {
                 panicf("Info too long!");
             }
             kstd::memcpy(header_ptr->infoPtr(), infoPtr, kstd::strlen(infoPtr)+1);
         } else {
             kstd::memset(header_ptr->infoPtr(), 0, HEADER_INFO_SIZE);
         }
         //TODO? Some safety check to make sure we are not overrunning another block?
         //Done
         return header_ptr;
    }

    uint8_t* MAllocator::memoryLimit() {
        return this->memory_base + this->memory_size;
    }

    MAllocator::header* MAllocator::nextHeader(header* current) {
        //The next header should start after the total size of this, so
        uint8_t* ptr = ((uint8_t*)current) + sizeof(header) + current->totalSize();
        //Check if it is within the memory bounds
        if(ptr >= this->memory_base && ptr < this->memoryLimit()) {
            //Read the header off the pointer
            header* h_ptr = (header*)ptr;
            //Check the magic
            h_ptr->check();
            //Return the header
            return h_ptr;
        }
        //otherwise, no next found
        return nullptr;
    }

    //Try to merge contigous headers
    void MAllocator::tryMerge(header* startHeader) {
        header* current = startHeader;
        //We can only merge free blocks!
        if(!current->free) {
            panicf("Attempting to merge left non-free block at address %x (data address %x)",
                (uint32_t)current, (uint32_t)current->dataPtr()
            );
        }
        while(true) {
            header* next = this->nextHeader(current);
            //There is a next header, and it is also free
            if(next != nullptr && next->free) {
                //Let's do the merge: add the total size of the block + header to the current data size
                //First carpet-bomb the next header
                current->dataSize += sizeof(header) + next->totalSize();
                next->carpet_with(0);
                //Current does not move in this process
            } else {
                //Done
                return;
            }
        }   
    }

    MAllocator::header* MAllocator::first() {
        return (header*)this->memory_base;
    }

    MAllocator::header* MAllocator::findFree(size_t size) {
        header* hdr = this->first();
        //Here we go
        while(hdr != nullptr) {
            //First, check that the current `header` is actually good
            hdr->check();
            //Is it free and big enough?
            if(hdr->free && hdr->dataSize >= size) {
                //Found it!
                return hdr;
            }
            //Not big enough, increment
            hdr = this->nextHeader(hdr);
        }
        //No free header left...
        return nullptr;
    }

    MAllocator::header* MAllocator::grow(size_t size) {
        //Figure out how many pages are needed.
            size_t numPages = size/memory::BIG_PAGE_SIZE;
            if(size % memory::BIG_PAGE_SIZE != 0) {
                numPages++;
            }
            //Allocated them -and they need to be contiguous!
            int prev = -1;
            int curr = -1;
            for(size_t i = 0; i < numPages; i++) {
                curr = this->pg_allocator->allocate();
                //If we have already seen a previous, and the previous is less then the current - 1
                //(not adjacent)
                if(i != 0 && prev != curr - 1) {
                    panicf("MAllocator failed - page allocator returned non-contiguous pages");
                }
            }
            //Grow the heap by the allocated size
            this->memory_size += numPages*memory::BIG_PAGE_SIZE;
            //recover the index of the first page allocated
            int firstPage = curr - numPages;
            //get the address of the page
            auto firstPagePtr = (uint8_t*)page_index_to_address(firstPage);
            //install the header in the block
            return this->makeBlock(firstPagePtr - this->memory_base, nullptr, size);
    }

    void* MAllocator::malloc(size_t size) {
        //logf("Requested size %x\n", size);
        if(this->memory_base == nullptr) {
            panicf("Attempting to allocate via unitialized allocator\n");
        }
        //Try to get a free header of the required size
        header* hdr = this->findFree(size);
        //If we failed, ask for enough new pages and try again
        if(hdr == nullptr) {
            hdr = this->grow(size);
        }

        //If the requested size is large enough, and the remaining size after a split is also large enough
        size_t splitSize = size;
        if(splitSize < MALLOCATOR_MIN_BLOCK_SIZE) {
            splitSize = MALLOCATOR_MIN_BLOCK_SIZE;
        }
        size_t remainderSize = hdr->totalSize() - splitSize;
        if(remainderSize > sizeof(header) + MALLOCATOR_MIN_BLOCK_SIZE) {
            //split
            //Shrink the current header
            hdr->dataSize = splitSize;
            //Generate the new block
            auto newHeaderPointer = hdr->dataPtr() + splitSize;
            
        
            this->makeBlock(newHeaderPointer - this->memory_base, nullptr, remainderSize - sizeof(header));
        }
        //Set as used
        hdr->free = false;
        if(this->tagSet) {
                this->tagSet = false;
                //TODO: DANGER DANGER - this is unchecked!
                kstd::memcpy(hdr->info, this->nextTag, kstd::strlen(this->nextTag)+1);
        }
        //Return the pointer to the hdr data
        //logf("At the end of malloc, we have allocated the following block\n");
        //logf("\n");
        return hdr->dataPtr();
    }


    void MAllocator::free(void* ptr) {
        //logf("Free called, debug begin:\n");
        //this->log_entry(ptr);
        //logf("Debug end\n");

        //The the block's header
        uint8_t* header_ptr = ((uint8_t*)(ptr)) - sizeof(header);
        header* hdr = (header*)header_ptr;
        hdr->free = true;
        //Remove the tag
        kstd::memset(hdr->info, 0, HEADER_INFO_SIZE);
        //Try to merge the header!
        this->tryMerge(hdr);
    }

    void MAllocator::setNextTag(char* tag) {
        int len = kstd::strlen(tag);
        if(len + 1 > HEADER_INFO_SIZE) {
            panicf("MAllocator - invalid tag - tag too long\n");
        }
        kstd::memcpy(this->nextTag, tag, len+1);
        this->tagSet = true;
    }

    MAllocator::MAllocator() {
        this->pg_allocator = nullptr;
        this->memory_base = nullptr;
        this->memory_size = 0;
    }
    
    MAllocator::MAllocator(page_allocator* pg_alloc) {
        this->pg_allocator = pg_alloc;
        int firstPage = this->pg_allocator->allocate();
        this->memory_base = (uint8_t*)(page_index_to_address(firstPage));
        this->memory_size = BIG_PAGE_SIZE - sizeof(header);
        this->tagSet = false;
        this->makeBlock(0, nullptr, this->memory_size - sizeof(header));
    }

    void MAllocator::log_state() {
        logf("--MAllocator DEBUG START--\n");
        logf("Memory base = %x, Memory size = %d bytes, memory end = %x\n", 
        this->memory_base, this->memory_size, sizeof(header) + this->memory_base + this->memory_size);
        logf("Showing blocks...\n");
        header* hdr = this->first();
        while(hdr != nullptr) {
            logf("Header address = %x, data address = %x, data size = %d B, free = %b tag = [%s]\n",
                (uint32_t)hdr, (uint32_t)hdr->dataPtr(), hdr->dataSize,  hdr->free, hdr->info);
            hdr = this->nextHeader(hdr);
        }
    }

    void MAllocator::log_entry(const void* ptr) {
        header* hdr =  (header*)(((uint8_t*)ptr) - sizeof(header));
        logf("Header address = %x, data address = %x, data size = %d B, free = %b tag = [%s]\n",
                (uint32_t)hdr, (uint32_t)hdr->dataPtr(), hdr->dataSize,  hdr->free, hdr->info);
    }
};