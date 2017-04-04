#include <kernel/serial.h>
#include <kernel/arch.h>
#include <kernel/std.h>

void serial_init(uint16_t port)
{
  outb(port + 1, 0x00);    // Disable all interrupts
  outb(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
  outb(port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
  outb(port + 1, 0x00);    //                  (hi byte)
  outb(port + 3, 0x03);    // 8 bits, no parity, one stop bit
  outb(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
  outb(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int is_transmit_empty(uint16_t port) {
   return inb(port + 5) & 0x20;
}

void serial_write(uint16_t port, uint8_t a) {
   while (is_transmit_empty(port) == 0);

   outb(port,a);
}

void serial_writestring(const char* str) {
  serial_writestring(str, SERIAL_DEFAULT_PORT);
}

void serial_writestring(const char* str, uint16_t port) {
  size_t len = k_strlen(str);
  for(size_t i = 0; i < len; i++) {
    serial_write(port, str[i]);
  }
}
