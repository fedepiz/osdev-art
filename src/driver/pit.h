#ifndef DRIVER_PIT_H
#define DRIVER_PIT_H
#include <stdint.h>
typedef void (*pit_callback_f)(uint32_t);
void pit_set_phase(int hz);
void pit_initialize(int hz, pit_callback_f callback);
#endif