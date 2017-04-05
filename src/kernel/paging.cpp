#include <kernel/paging.h>
#include <kernel/arch.h>
#include <kernel/std.h>

uint32_t* page_directory;

#define BIG_PAGE_SIZE 0x400000

void print_kernel_limits() {
    uint32_t kernelStart = (uint32_t)&kernel_start;
    uint32_t kernelEnd = (uint32_t)&kernel_end;
    k_debug_writestring("Kernel limits: ");
    k_debug_writestring(itoa(kernelStart,16).str);
    k_debug_writestring(" - ");
    k_debug_writestring(itoa(kernelEnd,16).str);
    k_debug_writestring("\n");
    k_debug_writestring("Kernel size: ");
    k_debug_writestring(itoa(kernel_size(),16).str);
    k_debug_writestring("\n");
}

void paging_debug() {
    k_debug_writestring("DEBUGGING PAGING\n");
    k_debug_writestring("Address of boot page directory is:");
    uint32_t bootPageDirectoryAddr = (uint32_t)&BootPageDirectory;
    k_debug_writestring(itoa(bootPageDirectoryAddr,16).str);
    k_debug_writestring("\n");
    print_kernel_limits();
}

int pageTableIndex(uint32_t virtaddrs) {
    return virtaddrs >> 22;
}

int bigPageSize(size_t size) {
    return (size/BIG_PAGE_SIZE+1);
}

void page_in_kernel() {
    //Compute the number of 4MB pages needed and the first page address
    int kernelBigPageSize = bigPageSize(kernel_size());
    int kernelBasePage = pageTableIndex(KERNEL_VIRTUAL_BASE);
    k_debug_writestring("Kernel needs the following number of 4MB pages...");
    k_debug_writestring(itoa(kernelBigPageSize).str);
    k_debug_writestring("\n");
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

void paging_initialize() {
    //The initial page is the boot page directory
    page_directory = (uint32_t*)&BootPageDirectory;
    //4MB identity page the kernel
    page_in_kernel();
}