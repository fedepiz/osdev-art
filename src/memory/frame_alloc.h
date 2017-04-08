#ifndef KERNEL_FRAME_ALLOC_H
#define KERNEL_FRAME_ALLOC_H

#include <stdint.h>
#include <stddef.h>

namespace memory {
    enum frame_state {
        FREE, ALLOCATED, RESERVED, UNAVAILABLE
    };
    int address_to_frame_index(void* addr);
    void* frame_index_to_address(int frame);
    int frames_in_range(size_t size);
    int first_free();
    int allocate_frame();
    void reserve_frame(int frame);
    void free_frame(int frame);
    frame_state state_of_frame(int frame);
    int available();
    void frame_alloc_initialize(uint32_t kernel_upper_address, uint32_t memory_upper_limit);
    void frame_alloc_debug(bool verbose);
};
#endif