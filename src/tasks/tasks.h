#ifndef TASKS_KERNEL_TASK_H
#define TASKS_KERNEL_TASK_H

#include <stdint.h>
#include <util/list.h>
#include <kernel/arch.h>

namespace tasks {
    using util::list;

    class Task {
        public:
        arch::regs regs;
        Task(void (*routine)(), unsigned int flags);
    };

    class TaskScheduler {
        private:
        list<Task*> taskQueue;
        Task* currentTask;
        void rotateTasks();
        public:
        TaskScheduler();
        void enqueueTask(Task* task);
        void executeSwitch(arch::regs* stackRegs);
    };

    extern TaskScheduler* taskScheduler;

    void taskSwitchHandler(arch::regs* regs);

    extern "C" void halt();
    typedef void (*kernelTask_t)();
    void launchKernelTask(kernelTask_t task);
    void initialize();
    void test();
};
#endif