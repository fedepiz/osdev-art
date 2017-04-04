#include <driver/pit.h>
#include <kernel/arch.h>

void pit_set_phase(int hz) {
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    outb(0x43, 0x36);             /* Set our command byte 0x36 */
    outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

pit_callback_f timerCallback;

uint32_t timer_ticks;

void timer_handler(struct regs *r) {
    (void)r;
    timer_ticks++;
    if(timerCallback != nullptr) {
        timerCallback(timer_ticks);
    }
}

void pit_initialize(int hz, pit_callback_f callback) {
    timer_ticks = 0;
    pit_set_phase(hz);
    timerCallback = callback;
    irq_install_handler(PIT_IRQ_CODE, timer_handler);
}