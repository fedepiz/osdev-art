#ifndef TASKS_SCHEDULER_H
#define TASKS_SCHEDULER_H

#include <util/list.h>
#include <tasks/Task.h>
namespace tasks {
    using util::list;

    class Scheduler {
        private:
        list<Task*> taskQueue;
        Task* currentTask;
        void rotateTasks();
        public:
        Scheduler();
        void enqueueTask(Task* task);
        void executeSwitch(arch::regs* stackRegs);
    };
};
#endif