#ifndef MEMORY_MEMORY_ALLOCATOR_H
#define MEMORY_MEMORY_ALLOCATOR_H

#include <stddef.h>
#include <util/text.h>

namespace memory {
    using util::logf;
    class MemoryAllocator {
        public:
        virtual void* malloc(size_t size) = 0;
        virtual void free(void* ptr) = 0;
        virtual void setNextTag(char* tag) = 0;
        virtual void log_state() = 0;
        virtual void log_entry(const void* ptr) = 0;
    };
};

#endif