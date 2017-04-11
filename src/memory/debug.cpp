#include <memory/debug.h>
#include <memory/frame_alloc.h>
#include <memory/paging.h>

namespace memory {
    void kernel_page_allocator_debug(bool verbose) {
        kernel_page_allocator.debug(verbose);
    }
    void kernel_frame_allocator_debug(bool verbose) {
        frame_alloc_debug(verbose);
    }
};