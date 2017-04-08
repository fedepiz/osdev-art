#include <memory/subsystem.h>
#include <memory/paging.h>
#include <memory/frame_alloc.h>
#include <util/DynamicHeap.h>
#include <memory/MAllocator.h>
#include <kernel/arch.h>

namespace memory {
    using util::logf;

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
            //kernel_heap_initialize();
            MAllocator alloc(&kernel_page_allocator);
            alloc.log_state();
            int* x = (int*)alloc.malloc(sizeof(int));
            int* y = (int*)alloc.malloc(sizeof(int));
            alloc.log_state();
            alloc.free(y);
            alloc.log_state();
            alloc.free(x);
            alloc.log_state();
            //int* y = (int*)alloc.malloc(sizeof(int));
            //uint8_t* ptr = (uint8_t*)alloc.malloc(128);
            //logf("%d %d %d\n", *x, *y, ptr[52]);
            //alloc.free(y); 
            //alloc.log_state();
            for(;;) {

            }
    }
};