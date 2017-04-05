#include <kernel/frame_alloc.h>
#include <kernel/std.h>
#include <kernel/arch.h>

#define FRAME_SIZE 0x400000
#define NUM_FRAMES 1024

//These are 4MB frames
bool frame_array[NUM_FRAMES];

int first_free_frame() {
    for(int i = 0; i < NUM_FRAMES;i++) {
        if(!frame_array[i]) {
            return i;
        }
    }
    k_panic("Out of frames!");
    return 0;
}

int allocate_frame() {
    int index = first_free_frame();
    frame_array[index] = true;
    return index;
}

void free_frame(int index) {
    frame_array[index] = false;
}

int available_frames() {
    int count = 0;
    for(int i = 0; i < NUM_FRAMES; i++) {
        if(!frame_array[i]) {
            count++;
        }
    }
    return count;
}

void frame_alloc_initialize() {
    //Kernel is physically allocated at 0, reserve that amount of frames
    int kernel_frame_count = (kernel_size()/FRAME_SIZE)+1;
    for(int i = 0; i < NUM_FRAMES; i++) {
        if(i <= kernel_frame_count) {
            frame_array[i] = true;
        } else {
            frame_array[i] = false;
        }
    }
}
