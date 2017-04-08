#ifndef MEMORY_MALLOCATOR_H
#define MEMORY_MALLOCATOR_H

#include <stdint.h>
#include <stddef.h>
#include <memory/paging.h>
#include <memory/MemoryAllocator.h>

namespace memory {

    class MAllocator : MemoryAllocator {
        private:
        static const int HEADER_INFO_SIZE = 5;
        struct header {
            uint16_t magic;
            size_t dataSize;
            bool free;
            char info[HEADER_INFO_SIZE];

            size_t totalSize();
            void check();
            char* infoPtr();
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
        
        header* nextHeader(header* current);
        header* makeBlock(size_t offset, char* infoPtr, size_t dataSize);
        void tryMerge(header* startHeader);
        header* first();
        header* findFree(size_t size);
        header* grow(size_t size);
        public:
        MAllocator(page_allocator* pg_alloc);
        virtual void* malloc(size_t size);
        virtual void free(void* ptr);
        void log_state();
    };
};

#endif