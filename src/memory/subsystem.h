#ifndef MEMORY_SUBSYSTEM_H
#define MEMORY_SUBSYSTEM_H

#include <kernel/multiboot.h>
#include <memory/MemoryAllocator.h>

namespace memory {
    MemoryAllocator* getKernelHeap();
    void initialize(multiboot::multiboot_info_t* mbinfo);
};

#endif