#ifndef KERNEL_FRAME_ALLOC_H
#define KERNEL_FRAME_ALLOC_H
int first_free();
int allocate_frame();
void free_frame(int frame);
int available_frames();
void frame_alloc_initialize();
#endif