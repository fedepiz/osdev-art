#ifndef UTIL_DYNAMIC_HEAP_H
#define UTIL_DYNAMIC_HEAP_H
#include <kernel/paging.h>
#include <util/heap_common.h>
#include <stdint.h>
#include <stddef.h>
namespace util {
    struct DynamicHeap {
        private:
        paging::page_allocator* page_allocator;
        uint8_t* memory_base;
        heap_common::heapBlockHeader* head_block;
        heap_common::heapBlockHeader* last_header();
        bool grow_page();
        bool grow(size_t count);
        public:
        void initialize(paging::page_allocator* page_allocator);
        void* malloc(size_t count);
        void free(void* ptr);
        void debug();
    };

    void DynamicHeap_initialize(DynamicHeap* heap, paging::page_allocator* page_allocator);
};
#endif