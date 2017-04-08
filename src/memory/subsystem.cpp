#include <memory/subsystem.h>
#include <memory/paging.h>
#include <memory/frame_alloc.h>
#include <memory/MAllocator.h>
#include <memory/MemoryAllocator.h>
#include <kernel/arch.h>

namespace memory {
    using util::logf;

    unsigned char kernel_heap_buffer[sizeof(MAllocator)];
    MemoryAllocator* kernel_heap;

    MemoryAllocator* getKernelHeap() {
        return kernel_heap;

    }

    void kernelAllocSetNextTag(const char* tag) {
        getKernelHeap()->setNextTag((char*)tag);
    }

    void kernelHeapLogState() {
        getKernelHeap()->log_state();
    }

    void kernelHeapLogEntry(const void* ptr) {
        getKernelHeap()->log_entry(ptr);
    }

    void kernel_heap_initialize() {
        kernel_heap = (MemoryAllocator*)kernel_heap_buffer;
        MAllocator local(&kernel_page_allocator);
        kstd::memcpy(kernel_heap, &local, sizeof(MAllocator));
    }

    void initialize(multiboot::multiboot_info_t* mbinfo) {
            uint32_t mem_size = mbinfo->mem_upper*1024; //1 KiB per unit
            frame_alloc_initialize(arch::kernel_size(), mem_size);
            multiboot::reserve_modules_frames(mbinfo);
            paging_initialize();  
            kernel_heap_initialize();
    }
};