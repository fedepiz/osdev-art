/* Surely you will remove the processor conditionals and this comment
   appropriately depending on whether or not you use C++. */
/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

#include <kernel/vga_terminal.h>
#include <kernel/serial.h>

extern "C" void kernel_main(void) {
	/* Initialize terminal interface */
	terminal_initialize();

	terminal_writestring("Hello, kernel World!\n");

  /* Initializes serial com 1, used for debugging */
	serial_init(SERIAL_COM1_BASE);
	serial_writestring("Testing the beauty of the serial port\n");

	terminal_writestring("Hanging...");
}
