#ifndef TASKS_KERNEL_TASK_H
#define TASKS_KERNEL_TASK_H

#include <stdint.h>
#include <util/list.h>
#include <kernel/arch.h>

#include <tasks/Task.h>
#include <tasks/Scheduler.h>

namespace tasks {
    extern Scheduler* taskScheduler;

    void taskSwitchHandler(arch::regs* regs);

    extern "C" void halt();
    void initialize();
    void test();
};
#endif