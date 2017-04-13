#ifndef TASKS_TASK_H
#define TASKS_TASK_H

#include <kernel/arch.h>

namespace tasks {
    class Task {
        public:
        arch::regs regs;
        Task(void (*routine)(), unsigned int flags);
    };
};
#endif