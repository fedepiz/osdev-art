#ifndef UTIL_DYNAMIC_HEAP_H
#define UTIL_DYNAMIC_HEAP_H

#include <memory/paging.h>
#include <util/heap_common.h>
#include <stdint.h>
#include <stddef.h>

namespace util {

    extern void* watched_ptr;
    struct DynamicHeap {
        private:
        memory::page_allocator* page_allocator;
        uint8_t* memory_base;
        heap_common::heapBlockHeader* head_block;
        heap_common::heapBlockHeader* last_header();
        bool is_chatty;
        bool grow_page();
        bool grow(size_t count);
        heap_common::heapBlockHeader* _free(void* ptr);
        public:
        void initialize(memory::page_allocator* page_allocator);
        void* malloc(size_t count);
        void free(void* ptr);
        void cfree(void* ptr);
        void debug();
        void chatty_mode(bool);
    };

    void DynamicHeap_initialize(DynamicHeap* heap, memory::page_allocator* page_allocator);
};
#endif