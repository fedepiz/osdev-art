#ifndef MEMORY_MALLOCATOR_H
#define MEMORY_MALLOCATOR_H

#include <stdint.h>
#include <stddef.h>
#include <memory/paging.h>


namespace memory {

    class MAllocator {
        private:
        struct header {
            uint16_t magic;
            size_t infoSize;
            size_t dataSize;
            bool free;

            size_t totalSize();
            void check();
            uint8_t* infoPtr();
            uint8_t* dataPtr();
            void carpet_with(uint8_t value);
        };
        private:
        //The page allocator
        page_allocator* pg_allocator;
        //Pointer to the base of managed memory
        uint8_t* memory_base;
        //Upper size of the managed region
        size_t memory_size;

        uint8_t* memoryLimit();
        
        MAllocator::header* nextHeader(header* current);
        MAllocator::header* makeBlock(size_t offset, size_t infoSize, uint8_t* infoPtr, size_t dataSize);
        void tryMerge(header* startHeader);
        header* first();
        header* findFree(size_t size);
        public:
        void* malloc(size_t size);
        void free(void* ptr);
    };
};

#endif