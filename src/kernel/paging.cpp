#include <kernel/paging.h>
#include <kernel/std.h>

void paging_debug() {
    k_debug_writestring("DEBUGGING PAGING\n");
    k_debug_writestring("Address of boot page directory is:");
    uint32_t bootPageDirectoryAddr = (uint32_t)&BootPageDirectory;
    k_debug_writestring(itoa(bootPageDirectoryAddr,16).str);
    k_debug_writestring("\n");
}

void paging_initialize() {
    
}