#include <tasks/tasks.h>
#include <memory/paging.h>
#include <kernel/arch.h>
#include <util/text.h>
#include <kstdio.h>

namespace tasks {
    using util::logf;

    Scheduler* currentTaskScheduler;

    void taskSwitchHandler(arch::regs* regs) {
        //Simply changing eip - not wise, but who cares!
        //regs->eip = (unsigned int)test; This seems to works, as in it lands us where we want

        if(currentTaskScheduler == nullptr) {
            return;
        }
        //currentTaskScheduler->executeSwitch(regs);
    }

    void initialize() {
        //disalbe this stuff for now
        //TaskScheduler* sched = new TaskScheduler();
        //currentTaskScheduler = sched;
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
        //Task* t1 = new Task(func1, arch::get_eflags());
        //currentTaskScheduler->enqueueTask(t1);
        //Task* t2 = new Task(func2, arch::get_eflags());
        //currentTaskScheduler->enqueueTask(t2);
    }
};