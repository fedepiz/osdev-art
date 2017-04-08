#include <memory/subsystem.h>
#include <memory/paging.h>
#include <memory/frame_alloc.h>
#include <util/DynamicHeap.h>
#include <kernel/arch.h>

namespace memory {
    util::DynamicHeap kernel_heap;

    util::DynamicHeap& getKernelHeap() {
        return kernel_heap;
    }

    void kernel_heap_initialize() {
        util::DynamicHeap_initialize(&kernel_heap, &memory::kernel_page_allocator);
    }

    void initialize(multiboot::multiboot_info_t* mbinfo) {
            uint32_t mem_size = mbinfo->mem_upper*1024; //1 KiB per unit
            frame_alloc_initialize(arch::kernel_size(), mem_size);
            multiboot::reserve_modules_frames(mbinfo);
            paging_initialize();    
            kernel_heap_initialize();
    }
};