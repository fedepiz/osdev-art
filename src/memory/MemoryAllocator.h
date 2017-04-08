#ifndef MEMORY_MEMORY_ALLOCATOR_H
#define MEMORY_MEMORY_ALLOCATOR_H

#include <stddef.h>

namespace memory {
    class MemoryAllocator {
        public:
        virtual void* malloc(size_t size) = 0;
        virtual void free(void* ptr) = 0;
    };
};

#endif