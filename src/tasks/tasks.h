#ifndef TASKS_KERNEL_TASK_H
#define TASKS_KERNEL_TASK_H

#include <stdint.h>
#include <util/list.h>
#include <kernel/arch.h>

namespace tasks {
    using util::list;

    struct Registers {
        uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags;
    };

    class Task {
        public:
        Registers regs;
        Task(void (*routine)(), unsigned int flags);
    };

    class TaskScheduler {
        private:
        list<Task*> tasks;
        Task* currentTask;
        public:
        TaskScheduler();
    };

    extern TaskScheduler* taskScheduler;

    void taskSwitchHandler(arch::regs* regs);

    extern "C" void halt();
    typedef void (*kernelTask_t)();
    void launchKernelTask(kernelTask_t task);
};
#endif