#ifndef KERNEL_FRAME_ALLOC_H
#define KERNEL_FRAME_ALLOC_H
#include <stdint.h>
#include <stddef.h>
namespace frame_alloc {
    enum frame_state {
        FREE, ALLOCATED, RESERVED, UNAVAILABLE
    };
    int address_to_frame_index(void* addr);
    void* frame_index_to_address(int frame);
    int frames_in_range(size_t size);
    int first_free();
    int allocate();
    void reserve(int frame);
    void free(int frame);
    int available();
    void initialize(uint32_t kernel_upper_address, uint32_t memory_upper_limit);
    void debug(bool verbose);
};
#endif