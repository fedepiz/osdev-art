#include <kernel/FrameAllocator.h>
#include <kernel/std.h>

frame_node* nodeOf(frame_node* head, uint32_t base) {
    while(head != nullptr) {
        if(head->base <= base && head->limit() >= base) {
            return head;
        }
        head = head->next;
    }
    return nullptr;
}