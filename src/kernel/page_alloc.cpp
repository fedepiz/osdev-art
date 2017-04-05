#include <kernel/page_alloc.h>
#include <kernel/frame_alloc.h>
#include <kernel/paging.h>
#include <kernel/std.h>

int first_free_page(page_allocator* alloc) {
    for(size_t i = 0; i < alloc->pages_count; i++) {
        if(!alloc->pages_array[i]) {
            return i;
        }
    }
    return -1;
}

int page_allocator::allocate() {
    //Find the first available free page
    int free_index = first_free_page(this);
    if(free_index == -1) {
        k_panic("Cannot allocate page - allocator out of memory");
        return -1;
    }
    //Ask the frame allocator for a free frame
    int frame_index = allocate_frame();
    //Mark the free page as taken
    this->pages_array[free_index] = true;
    //Map the page to the given frame
    map_page(free_index, frame_index);
    return free_index;
}

void page_allocator::free(int page) {
    if(!this->pages_array[page]){
        k_panic("Attempting to free unused page");
    }
    //Get the index of the frame currently mapped to this page
    int frame_index = frame_of_page(page);
    free_frame(frame_index);
    //Unmap the page
    unmap_page(page);
}