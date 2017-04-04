#ifndef KERNEL_PAGING_H
#define KERNEL_PAGING_H

extern "C" void BootPageDirectory();//Dummy extern symbol, not a real function


void paging_debug();
void paging_initialize();
#endif