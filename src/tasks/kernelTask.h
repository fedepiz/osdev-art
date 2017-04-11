#ifndef TASKS_KERNEL_TASK_H
#define TASKS_KERNEL_TASK_H

namespace tasks {
    typedef void (*kernelTask_t)();
    void launchKernelTask(kernelTask_t task);
};
#endif