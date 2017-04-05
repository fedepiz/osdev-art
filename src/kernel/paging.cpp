#include <kernel/paging.h>
#include <kernel/arch.h>
#include <kernel/kstd.h>

namespace paging {
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

    void page_in_kernel() {
        //Compute the number of 4MB pages needed and the first page address
        int kernelBigPageSize = bigPageSize(arch::kernel_size());
        int kernelBasePage = pageTableIndex(arch::KERNEL_VIRTUAL_BASE);
        log("Kernel needs the following number of 4MB pages...");
        log(itoa(kernelBigPageSize).str);
        log("\n");
        for(int i = 0; i < kernelBigPageSize; i++) {
            //Set R/W 4 MB page corresponding to the ith physical address
            map_page(kernelBasePage+i, i);
        }
    }

    void map_page(int pageIndex, int frameIndex) {
        page_directory[pageIndex] = 0x00000083 | frameIndex << 12;
    }

    int frame_of_page(int pageIndex) {
        return page_directory[pageIndex] >> 12;
    }

    bool page_is_mapped(int pageIndex) {
        return page_directory[pageIndex] != 0x00000002 || page_directory[pageIndex] != 0;
    }

    void unmap_page(int pageIndex) {
        page_directory[pageIndex] = 0x00000002;
    }

    void debug() {
        log("DEBUGGING PAGING\n");
        log("Address of boot page directory is:");
        uint32_t bootPageDirectoryAddr = (uint32_t)&BootPageDirectory;
        log(itoa(bootPageDirectoryAddr,16).str);
        log("\n");
        print_kernel_limits();
    }


    void initialize() {
        //The initial page is the boot page directory
        page_directory = (uint32_t*)&BootPageDirectory;
        //4MB identity page the kernel
        page_in_kernel();
    }
};