#include <memory/MAllocator.h>
#include <kstdlib.h>
#include <util/text.h>

namespace memory {
    using util::panicf;
    using util::logf;

    size_t MAllocator::header::totalSize() {
        return this->infoSize + this->dataSize;
    }

    uint8_t* MAllocator::header::infoPtr() {
        auto this_ptr = (uint8_t*)this;
        return this_ptr + sizeof(header);
    }

    uint8_t* MAllocator::header::dataPtr() {
        auto this_ptr = (uint8_t*)this;
        return this_ptr + sizeof(header) + this->infoSize;
    }

    void MAllocator::header::check() {
        if(this->magic != 0xFEDE) {
            panicf(
                "MAllocator header at address %x (data address %x), has invalid magic number %x",
                (uint32_t)this, (uint32_t)this->dataPtr(), this->magic
            );
        }
    }

    void MAllocator::header::carpet_with(uint8_t value) {
        kstd::memset(this, value, sizeof(header) + this->totalSize());
    }

    MAllocator::header* MAllocator::makeBlock(size_t offset, size_t infoSize, uint8_t* infoPtr, size_t dataSize) {
        //Compute the actual buffer
        uint8_t* buffer = this->memory_base + offset;
         //The header goes right at the start
         header* header_ptr = (header*)buffer;
         //Then goes the info section
         uint8_t* info_buffer = header_ptr->infoPtr();
         //finally, we want the pointer to the data, so
         uint8_t* data_buffer = header_ptr->dataPtr();

         //Populate the header information
         header_ptr->magic = 0xFEDE;
         header_ptr->infoSize = infoSize;
         header_ptr->dataSize = dataSize;
         header_ptr->free = true;
         //Copy the information into the info region
         kstd::memcpy(info_buffer, infoPtr, infoSize);

         //TODO? Some safety check to make sure we are not overrunning another block?
         (void)data_buffer;
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

    void* MAllocator::malloc(size_t size) {
        //Try to get a free header of the required size
        header* hdr = this->findFree(size);
        //If we failed, ask for enough new pages and try again
        if(hdr == nullptr) {
            //this->pg_allocator->allocate(size);
        }
    }
    
    void MAllocator::free(void* ptr) {

    }
};