#ifndef KERNEL_ARCH_H
#define KERNEL_ARCH_H

#include <stdint.h>

    /** outb:
     *  Sends the given data to the given I/O port. Defined in io.s
     *
     *  @param port The I/O port to send the data to
     *  @param data The data to send to the I/O port
     */
    extern "C" void outb(uint16_t port, uint8_t data);
    extern "C" char inb(uint16_t port);
#endif
