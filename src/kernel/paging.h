#ifndef KERNEL_PAGING_H
#define KERNEL_PAGING_H
#include <stddef.h>
#include <stdint.h>

extern "C" void BootPageDirectory();//Dummy extern symbol, not a real function

namespace paging {
    const size_t BIG_PAGE_SIZE = 0x400000;


    enum page_state { free, reserved, allocated };


    int address_to_page_index(void* addr);
    void* page_index_to_address(int page);

    void map_page(int pageIndex, int frameIndex);
    void unmap_page(int pageIndex);
    bool page_is_mapped(int pageIndex);
    int frame_of_page(int pageIndex);
    
    void debug();
    void initialize();


    struct page_allocator {
    public:
        page_state* pages_array;
        size_t pages_count;
        int base_page;
        char name[16];
        int allocate();
        void free(int page);
        void reserve(unsigned int page);
        void debug(bool verbose);
    private:
        int first_free();
        void debug_page(int index);
    };

    extern page_allocator kernel_page_allocator;
};
#endif