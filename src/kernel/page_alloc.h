#ifndef KERNEL_PAGE_ALLOC_H
#define KERNEL_PAGE_ALLOC_H
#include <stdint.h>
#include <stddef.h>

struct page_allocator {
    bool* pages_array;
    size_t pages_count;
    int starting_page;
    public:
    int allocate();
    void free(int page);
};
#endif