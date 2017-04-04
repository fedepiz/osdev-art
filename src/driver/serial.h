#ifndef KERNEL_SERIAL_H
#define KERNEL_SERIAL_H
#include <stdint.h>

    /* The I/O ports */

    /* All the I/O ports are calculated relative to the data port. This is because
     * all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
     * order, but they start at different values.
     */

#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

#define SERIAL_LINE_ENABLE_DLAB          0x80

#define SERIAL_DEFAULT_PORT SERIAL_COM1_BASE

void serial_init(uint16_t port);
void serial_write(uint16_t port, uint8_t a);
void serial_writestring(const char* str);
void serial_writestring(const char* str, uint16_t port);

#endif
