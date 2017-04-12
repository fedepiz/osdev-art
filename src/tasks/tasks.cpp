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
        this->regs.esp = (unsigned int)new unsigned char[memory::BIG_PAGE_SIZE];
        this->regs.ebp = this->regs.esp;
        this->regs.eip = (unsigned int)routine;
    }

    TaskScheduler* taskScheduler;

    void test() {
        int x = 2;
        logf("Test ran! %d\n", x);
        for(;;) {

        }
    }

    void taskSwitchHandler(arch::regs* regs) {
        //Simply changing eip - not wise, but who cares!
        //regs->eip = (unsigned int)test; This seems to works, as in it lands us where we want

        if(taskScheduler == nullptr) {
            return;
        }

        //Update the current task with the regsiters in regs
        
        //Selet next task to run

        //Update regs with the values from that task
    }
};