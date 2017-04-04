#include <kernel/paging.h>
#include <kernel/arch.h>
#include <kernel/std.h>

void print_kernel_limits() {
    uint32_t kernelStart = (uint32_t)&kernel_start;
    uint32_t kernelEnd = (uint32_t)&kernel_end;
    k_debug_writestring("Kernel limits: ");
    k_debug_writestring(itoa(kernelStart,16).str);
    k_debug_writestring(" - ");
    k_debug_writestring(itoa(kernelEnd,16).str);
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

void paging_initialize() {
    
}