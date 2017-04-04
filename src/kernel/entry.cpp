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
#include <kernel/arch.h>
#include <kernel/globals.h>
#include <kernel/std.h>
#include <util/StaticHeap.h>

#define TEST_HEAP_SIZE (1024*2)
uint8_t heap_buffer[TEST_HEAP_SIZE];
StaticHeap* kernelHeap;

void initialize() {
	/* Initialize the global function dispatch selector */
	struct globals globals;
	globals.error_writestring = &terminal_writestring;
	globals.out_writestring = &terminal_writestring;
	globals.debug_writestring = &serial_writestring;
	/* Complete global initialization */
	globals_initialize(globals);
	/* Initialize the kernel heap */
	k_heap_initialize();
	/* Initialize terminal interface */
	terminal_initialize();
	terminal_writestring("Terminal initialized");

  /* Initializes serial com 1, used for debugging */
	serial_init(SERIAL_COM1_BASE);
	serial_writestring("Testing the beauty of the serial port\n");
	arch_initialize();
}

void hang() {
	k_debug_writestring("Hanging...\n");
	for(;;) {

	}
}

void testStaticHeap() {
	k_debug_writestring("Testing static heap\n");
	StaticHeap lclHeap(heap_buffer, TEST_HEAP_SIZE);
	lclHeap.setStrict(true);
	k_debug_writestring("INITIAL HEAP STATE\n");
	lclHeap.debug();
	auto* array1 = (uint32_t*)lclHeap.malloc(sizeof(uint32_t)*4);
	auto* array2 = (uint32_t*)lclHeap.malloc(sizeof(uint32_t)*4);
	array1[0] = 52;
	array2[0] = 24;
	k_debug_writestring("This should be 52: ");
	k_debug_writestring(itoa(array1[0]).str);
	k_debug_writestring("\n");
	heapSize heapSize = lclHeap.getSize();
	k_debug_writestring("Free size: ");
	k_debug_writestring(itoa(heapSize.free).str);
	k_debug_writestring("\nAllocated size: ");
	k_debug_writestring(itoa(heapSize.allocated).str);
	k_debug_writestring("\nAFTER ALLOCATIONS\n");
	lclHeap.debug();
	lclHeap.free(array2);
	k_debug_writestring("AFTER ONE FREE\n");
	lclHeap.debug();
	lclHeap.free(array1);
	k_debug_writestring("AFTER TWO FREES\n");
	lclHeap.debug();
}


extern "C" void kernel_main(void) {
	initialize();
	testStaticHeap();
	terminal_writestring("Hello, kernel World!\n");

	hang();
}
