#ifndef KERNEL_PAGING_H
#define KERNEL_PAGING_H

extern "C" void BootPageDirectory();//Dummy extern symbol, not a real function


void paging_debug();
void map_page(int pageIndex, int frameIndex);
void unmap_page(int pageIndex);
void paging_initialize();
#endif