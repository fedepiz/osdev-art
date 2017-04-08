#ifndef MEMORY_SUBSYSTEM_H
#define MEMORY_SUBSYSTEM_H

#include <util/DynamicHeap.h>
#include <kernel/multiboot.h>

namespace memory {
    util::DynamicHeap& getKernelHeap();
    void initialize(multiboot::multiboot_info_t* mbinfo);
};

#endif