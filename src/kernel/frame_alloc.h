#ifndef KERNEL_FRAME_ALLOC_H
#define KERNEL_FRAME_ALLOC_H
#include <stdint.h>
#include <stddef.h>
namespace frame_alloc {
    enum frame_state {
        FREE, ALLOCATED, RESERVED
    };
    int address_to_frame_index(void* addr);
    void* frame_index_to_address(int frame);
    int frames_in_range(size_t size);
    int first_free();
    int allocate();
    void reserve(int frame);
    void free(int frame);
    int available();
    void initialize(uint32_t upperAddress);
    void debug(bool verbose);
};
#endif