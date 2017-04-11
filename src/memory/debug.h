#ifndef MEMORY_DEBUG_H
#define MEMORY_DEBUG_H

namespace memory {
    void kernelAllocSetNextTag(const char* tag);
    void kernelHeapLogState();
    void kernelHeapLogEntry(const void* ptr);
    void set_memory_allocator_debug_mode(bool state);

    void kernel_page_allocator_debug(bool verbose);
    void kernel_frame_allocator_debug(bool verbose);    
};

#endif