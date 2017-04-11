#include <kstdio.h>
#include <kstdlib.h>
#include <memory/frame_alloc.h>
#include <memory/paging.h>

namespace memory {
    #define FRAME_SIZE 0x400000
    #define NUM_FRAMES 1024

    //These are 4MB frames
    frame_state frame_array[NUM_FRAMES];

    int address_to_frame_index(void* addr) {
        uint32_t addr_n = (uint32_t)addr;
        return addr_n >> 22;
    }

    void* frame_index_to_address(int frame) {
        return (void*)(frame << 22);
    }

    int frames_in_range(size_t size) {
        return (size/FRAME_SIZE)+1;
    }

    int first_free() {
        for(int i = 0; i < NUM_FRAMES;i++) {
            if(frame_array[i] == frame_state::FREE) {
                return i;
            }
        }
        return -1;
    }

    frame_state state_of_frame(int frame) {
        return frame_array[frame];
    }

    int allocate_frame() {
        int index = first_free();
        if(index == -1) {
            return -1;
        }
        frame_array[index] = frame_state::ALLOCATED;
        return index;
    }

    void reserve_frame(int index) {
        if(frame_array[index] == frame_state::ALLOCATED) {
            panic("Attempting to reserve already allocated frame");
        }
        else if(frame_array[index] == frame_state::RESERVED) {
            kstd::log("Double reserve request for frame of index ");
            kstd::log(kstd::itoa(index).str);
            kstd::log("\n");
        }
        frame_array[index] = frame_state::RESERVED;
    }

    void free_frame(int index) {
        //If the frame is already free...
        if(frame_array[index] == frame_state::FREE) {
            panic("Cannot free already free frame");
        }
        frame_array[index] = frame_state::FREE;
    }

    int available() {
        int count = 0;
        for(int i = 0; i < NUM_FRAMES; i++) {
            if(frame_array[i] == frame_state::FREE) {
                count++;
            }
        }
        return count;
    }

    struct frame_allocator_count {
        unsigned int free;
        unsigned int reserved;
        unsigned int allocated;
        unsigned int unavailable;
    };

    frame_allocator_count count_frames() {
        frame_allocator_count count;
        count.free = 0;
        count.reserved = 0;
        count.allocated = 0;

        for(int i = 0; i < NUM_FRAMES; i++) {
            frame_state state = frame_array[i];
            if(state == frame_state::ALLOCATED)
                count.allocated++;
            else if(state == frame_state::RESERVED)
                count.reserved++;
            else
                count.free++;
        }

        return count;
    }

    void debug_frame(int i) {
        frame_state state = frame_array[i];
        uint32_t physical_addr = (uint32_t)frame_index_to_address(i);
        kstd::log("(");
        kstd::log(kstd::itoa(i).str);
        kstd::log(")");
        kstd::log(kstd::itoa(physical_addr,16).str);
        kstd::log(" -> ");
        if(state == frame_state::ALLOCATED) {
            kstd::log("ALLOC");
        } else if (state == frame_state::FREE) {
            kstd::log("FREE");
        } else if (state == frame_state::RESERVED) {
            kstd::log("RESERVED");
        } else if (state == frame_state::UNAVAILABLE) {
            kstd::log("UNAVAILABLE");
        }
        kstd::log("\n");
    }

    void frame_alloc_debug(bool verbose) {
        frame_allocator_count count = count_frames();
        kstd::log("DEBUGGING FRAME ALLOCATOR\n");
        kstd::log("Free: ");
        kstd::log(kstd::itoa(count.free).str);
        kstd::log("\nAllocated: ");
        kstd::log(kstd::itoa(count.allocated).str);
        kstd::log("\nReserved: ");
        kstd::log(kstd::itoa(count.reserved).str);
        kstd::log("\n");

        if(verbose) {
            for (int i = 0; i < NUM_FRAMES; i++) {
                debug_frame(i);
            }
        }
    }



    void frame_alloc_initialize(uint32_t kenrel_upper_address, uint32_t upper_mem_limit) {
        //Kernel is physically allocated at 0, reserve that amount of frames
        int kernel_frame_count = frames_in_range(kenrel_upper_address);
        int mem_max_index = frames_in_range(upper_mem_limit);
        for(int i = 0; i < NUM_FRAMES; i++) {
            if(i <= kernel_frame_count) {
                frame_array[i] = frame_state::RESERVED;
            } else if (i >= mem_max_index){
               frame_array[i] = frame_state::UNAVAILABLE;
            } 
            else {
                frame_array[i] = frame_state::FREE;
            }
        }
    }
};
