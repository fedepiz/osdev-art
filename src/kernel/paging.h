#ifndef KERNEL_PAGING_H
#define KERNEL_PAGING_H
#include <stddef.h>

extern "C" void BootPageDirectory();//Dummy extern symbol, not a real function

namespace paging {


    const size_t BIG_PAGE_SIZE = 0x400000;

    void map_page(int pageIndex, int frameIndex);
    void unmap_page(int pageIndex);
    bool page_is_mapped(int pageIndex);
    int frame_of_page(int pageIndex);
    
    void debug();
    void initialize();
};
#endif