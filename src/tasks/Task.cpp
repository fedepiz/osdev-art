#include <tasks/Task.h>

namespace tasks {
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
};