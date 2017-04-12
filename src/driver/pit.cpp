#include <driver/pit.h>
#include <kernel/arch.h>
#include <tasks/tasks.h>
#include <util/text.h>

namespace pit {

using arch::outb;

void set_phase(int hz) {
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    outb(0x43, 0x36);             /* Set our command byte 0x36 */
    outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

pit_callback_f timerCallback;

uint32_t timer_ticks;

void timer_handler(struct arch::regs *r) {
    (void)r;
    timer_ticks++;
    //if(secret == 0) {
    //    util::logf("Performing the dirty swap\n");
    //    tasks::taskSwitchHandler(r);
    //    secret++;
    //}
    if(timerCallback != nullptr) {
        timerCallback(timer_ticks, r);
    }
}

void initialize(int hz, pit_callback_f callback) {
    timer_ticks = 0;
    set_phase(hz);
    timerCallback = callback;
    arch::irq::install_handler(PIT_IRQ_CODE, timer_handler);
}
};