#include <memory/paging.h>
#include <kernel/arch.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <memory/frame_alloc.h>

namespace memory {
    using kstd::log;
    using kstd::itoa;

    
    uint32_t* page_directory;

    void print_kernel_limits() {
      uint32_t kernelStart = (uint32_t)&arch::kernel_start;
      uint32_t kernelEnd = (uint32_t)&arch::kernel_end;
      log("Kernel limits: ");
      log(itoa(kernelStart,16).str);
      log(" - ");
      log(itoa(kernelEnd,16).str);
      log("\n");
      log("Kernel size: ");
      log(itoa(arch::kernel_size(),16).str);
      log("\n");
    }

    int pageTableIndex(uint32_t virtaddrs) {
        return virtaddrs >> 22;
    }

    int bigPageSize(size_t size) {
        return (size/BIG_PAGE_SIZE+1);
    }

    int address_to_page_index(void* addr) {
        uint32_t addr_n = (uint32_t)addr;
        return addr_n >> 22;
    }

    void* page_index_to_address(int page) {
        return (void*)(page << 22);
    }


    void page_in_kernel() {
        int kernelFirstPage = address_to_page_index((void*)&arch::kernel_start);
        int kernelLastPage = address_to_page_index((void*)&arch::kernel_end);
        int kernelPageCount = kernelLastPage - kernelFirstPage + 1;
        //Compute the number of 4MB pages needed and the first page address
        log("Kernel needs the following number of 4MB pages...");
        log(itoa(kernelPageCount).str);
        log("\n");
        for(int i = 0; i < kernelPageCount; i++) {
            //Set R/W 4 MB page corresponding to the ith physical address
            map_page(kernelFirstPage+i, i);
        }
    }

    void map_page(int pageIndex, int frameIndex) {
        page_directory[pageIndex] = 0x00000083 | frameIndex << 22;
    }

    int frame_of_page(int pageIndex) {
        return page_directory[pageIndex] >> 22;
    }

    bool page_is_mapped(int pageIndex) {
        return page_directory[pageIndex] != 0x00000002 || page_directory[pageIndex] != 0;
    }

    void unmap_page(int pageIndex) {
        page_directory[pageIndex] = 0x00000002;
    }
    
    int page_allocator::first_free() {
        for(size_t i = 0; i < this->pages_count; i++) {
            if(this->pages_array[i] == page_state::free) {
                return i;
            }
        }
        return -1;
    }

    int page_allocator::allocate() {
        //Find the first available free page
        int free_index = this->first_free();
        if(free_index == -1) {
            panic("Cannot allocate page - allocator out of memory");
            return -1;
        }
        //Ask the frame allocator for a free frame
        int frame_index = allocate_frame();
        //Fail if no frame is there...
        if(frame_index == -1) {
            return -1;
        }
        //Mark the free page as taken
        this->pages_array[free_index] = page_state::allocated;
        //Map the page to the given frame
        int page = free_index + this->base_page;
        map_page(page, frame_index);
        return page;
    }

    void page_allocator::reserve(unsigned int page) {
        unsigned int page_index = page - this->base_page;
        //If the index is valid
        if(page_index < this->pages_count) {
            //And the page is free...
            if(this->pages_array[page_index] == page_state::free) {
                //Reserve
                this->pages_array[page_index] = page_state::reserved;
            } else {
                panic("Cannot reserve page - page is not free");
            }
        } else {
            panic("Cannot reserve page - invalid index");
        }
    }

    void page_allocator::free(int page) {
        int page_index = page - this->base_page;
        if(page_index < 0 || (unsigned int)page_index >= this->pages_count) {
            panic("Invalid page number");
        }
        if(!this->pages_array[page_index]){
            panic("Attempting to free unused page");
        }
        //Get the index of the frame currently mapped to this page
        int frame_index = frame_of_page(page);
        free_frame(frame_index);
        //Unmap the page
        unmap_page(page);
        //set the state to free
        this->pages_array[page_index] = page_state::free;
    }

    void page_allocator::debug(bool verbose) {
        log("DEBUGGING PAGE ALLOCATOR\n");
        log("name: ");
        log(this->name);
        log("\nbase_page: ");
        log(kstd::itoa(this->base_page).str);
        log(" - virtual address: ");
        auto base_page_addr = (uint32_t)page_index_to_address(this->base_page);
        log(kstd::itoa(base_page_addr,16).str);
        log("\npages_count: ");
        log(kstd::itoa(this->pages_count).str);
        log("\n");

        if(verbose) {
            for(size_t i = 0; i < this->pages_count; i++) {
                this->debug_page(i);
            }
        }
    }

    void page_allocator::debug_page(int index) {
        auto absolute_index = this->base_page + index;
        auto page_addr = (uint32_t)page_index_to_address(absolute_index);
        log(kstd::itoa(index).str);
        log(" (");
        log(kstd::itoa(absolute_index).str);
        log(")");
        log(" -> ");
        log(kstd::itoa(page_addr,16).str);
        if(this->pages_array[index] == page_state::allocated) {
            log(" ALLOC\n");
        } else if(this->pages_array[index] == page_state::free) {
            log(" FREE\n");
        } else {
            log(" RESERVED\n");
        }
    }

    void debugPageTable() {
        log("DEBUGGING PAGING\n");
        log("Address of boot page directory is:");
        uint32_t bootPageDirectoryAddr = (uint32_t)&BootPageDirectory;
        log(itoa(bootPageDirectoryAddr,16).str);
        log("\n");
        print_kernel_limits();
    }

    page_allocator kernel_page_allocator;
    page_state kernel_page_allocator_pages_array[1024];

    void kernel_allocator_initialize() {
        int kernel_start_page = address_to_page_index((void*)&arch::kernel_start);
        int pages_to_end_of_memory = 1024 - kernel_start_page;
        int pages_to_reserve = address_to_page_index((void*)&arch::kernel_end) - kernel_start_page + 1;
        
        const char* name = "KRNL";
        kstd::memcpy(kernel_page_allocator.name, name, kstd::strlen(name));
        kernel_page_allocator.pages_array = kernel_page_allocator_pages_array;
        kernel_page_allocator.base_page = kernel_start_page;
        kernel_page_allocator.pages_count = pages_to_end_of_memory;

        for(int i = 0; i < pages_to_reserve;i++) {
            kernel_page_allocator.reserve(kernel_start_page+i);
        }
    }

    void kernel_page_allocator_debug(bool verbose) {
        memory::kernel_page_allocator.debug(verbose);
    }

    void paging_initialize() {
        //The initial page is the boot page directory
        page_directory = (uint32_t*)&BootPageDirectory;
        //4MB identity page the kernel
        page_in_kernel();
        //Initialize the kernel page allocator
        kernel_allocator_initialize();
    }
};