#include <tasks/tasks.h>
#include <memory/paging.h>
#include <kernel/arch.h>
#include <util/text.h>
#include <kstdio.h>


namespace tasks {
    using util::logf;

    extern "C" void jump_to_address(kernelTask_t task);

    void launchKernelTask(kernelTask_t task) {
        jump_to_address(task);
    }

    Task::Task(void (*routine)(), unsigned int eflags) {
        this->regs.eax = 0;
        this->regs.ebx = 0;
        this->regs.ecx = 0;
        this->regs.edx = 0;
        this->regs.esi = 0;
        this->regs.edi = 0;
        this->regs.eflags = eflags;
        this->regs.esp = (unsigned int)new unsigned char[0x1000];
        this->regs.ebp = this->regs.esp;
        this->regs.eip = (unsigned int)routine;
    }

    TaskScheduler::TaskScheduler() {
        this->currentTask = nullptr;
    }

    void TaskScheduler::enqueueTask(Task* task) {
        //Stop interrupts
        __asm__ __volatile__ ("cli");
        //If we have no current task
        if(this->currentTask == nullptr) {
            this->currentTask = task;
        } else {
            taskQueue.append(task);
        }
        //Resume interrupts
        __asm__ __volatile__("sti");
    }

    void TaskScheduler::rotateTasks() {
        //WARNING: MUST BE CALLED IN CLI/STI section

        //If we already have a current task, enqueue it
        if(this->currentTask != nullptr) {
            this->taskQueue.append(currentTask);
        }
        //dequeue a task from the list. if the list is empty, then set the task to none
        if(this->taskQueue.length() == 0) {
            this->currentTask = nullptr;
        } else {
            this->currentTask = taskQueue.behead();
        }
    }

    void TaskScheduler::executeSwitch(arch::regs* stackRegs) {
        __asm__ __volatile__ ("cli");
        if(this->currentTask != nullptr) {
            //If we have no current task, for now simply ignore the registers. Later we might want to 
            //sneakingly make a special task to recover to
            this->currentTask->regs = *stackRegs;
        }
        //Execute the task selection algorithm, after this if we have a new current task,
        //we will want to execute it
        this->rotateTasks();
        if(this->currentTask != nullptr) {
            //Update the stack regs with the values from the task
            *stackRegs = this->currentTask->regs;
        }
        //Here, the switch should have happened
        __asm__ __volatile__ ("sti");
    }

    TaskScheduler* currentTaskScheduler;

    void taskSwitchHandler(arch::regs* regs) {
        //Simply changing eip - not wise, but who cares!
        //regs->eip = (unsigned int)test; This seems to works, as in it lands us where we want

        if(currentTaskScheduler == nullptr) {
            return;
        }
        currentTaskScheduler->executeSwitch(regs);
    }

    void initialize() {
        TaskScheduler* sched = new TaskScheduler();
        currentTaskScheduler = sched;
    }

    void func1() {
        int x = 1;
        logf("Test ran! %d\n", x);
        for(;;) {

        }
    }

    void func2() {
        int x = 2;
        logf("Running func %d\n", x);
        for(;;) {

        }
    }

    void test() {
        Task* t1 = new Task(func1, arch::get_eflags());
        currentTaskScheduler->enqueueTask(t1);
        //Task* t2 = new Task(func2, arch::get_eflags());
        //currentTaskScheduler->enqueueTask(t2);
    }
};