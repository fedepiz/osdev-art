#ifndef DRIVER_PIT_H
#define DRIVER_PIT_H

#include <stdint.h>
#include <kernel/arch.h>
namespace pit {

const int DEFAULT_FREQUENCY_HZ = 100000;

typedef void (*pit_callback_f)(uint32_t, arch::regs*);
void set_phase(int hz);
void initialize(int hz, pit_callback_f callback);
};
#endif