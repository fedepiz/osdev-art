#include <kernel/page_alloc.h>
#include <kernel/frame_alloc.h>
#include <kernel/paging.h>
#include <kernel/kstd.h>

int page_allocator::first_free() {
    for(size_t i = 0; i < this->pages_count; i++) {
        if(!this->pages_array[i]) {
            return i;
        }
    }
    return -1;
}

int page_allocator::allocate() {
    //Find the first available free page
    int free_index = this->first_free();
    if(free_index == -1) {
        kstd::panic("Cannot allocate page - allocator out of memory");
        return -1;
    }
    //Ask the frame allocator for a free frame
    int frame_index = allocate_frame();
    //Mark the free page as taken
    this->pages_array[free_index] = true;
    //Map the page to the given frame
    paging::map_page(free_index, frame_index);
    return free_index;
}

void page_allocator::free(int page) {
    if(!this->pages_array[page]){
        kstd::panic("Attempting to free unused page");
    }
    //Get the index of the frame currently mapped to this page
    int frame_index = paging::frame_of_page(page);
    free_frame(frame_index);
    //Unmap the page
    paging::unmap_page(page);
}