#ifndef MEMORY_DEBUG_H
#define MEMORY_DEBUG_H

namespace memory {
    void kernelAllocSetNextTag(const char* tag);
    void kernelHeapLogState();
    void kernelHeapLogEntry(const void* ptr);
};

#endif