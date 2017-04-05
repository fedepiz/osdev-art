#ifndef KERNEL_FRAMEALLOCATOR_H
#define KERNEL_FRAMEALLOCATOR_H
#include <stdint.h>
#include <stddef.h>
struct frame_node {
    uint32_t base;
    size_t size;
    frame_node* next;
    bool isFree;

    uint32_t limit() {
        return this->base + this->size;
    }
};

class FrameAllocator {
    private:
    frame_node headNode;
    public:
};
#endif