#include <kernel/kstd.h>
#include <util/DynamicHeap.h>

namespace kstd {
util::DynamicHeap kernel_heap;

void memory_initialize() {
    util::DynamicHeap_initialize(&kernel_heap, &paging::kernel_page_allocator);
}

void* malloc(size_t count) {
    return kernel_heap.malloc(count);
}
void free(void* ptr) {
    kernel_heap.free(ptr);
}

void* calloc(size_t count) {
    void* ptr = kernel_heap.malloc(count);
    memset(ptr, 0, count);
    return ptr;
}
};


void *operator new(size_t size) {
    return kstd::calloc(size);
}
 
void *operator new[](size_t size) {
    return kstd::calloc(size);
}
 
void operator delete(void *p) {
    kstd::free(p);
}
 
void operator delete[](void *p) {
    kstd::free(p);
}

