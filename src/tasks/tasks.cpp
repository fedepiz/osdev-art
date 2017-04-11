#include <tasks/tasks.h>


namespace tasks {
    
    extern "C" void jump_to_address(kernelTask_t task);

    void launchKernelTask(kernelTask_t task) {
        jump_to_address(task);
    }
};