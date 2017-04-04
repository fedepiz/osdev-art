#include <kernel/arch.h>
#include <kernel/globals.h>
#include <kernel/std.h>
#include <util/StaticHeap.h>
#include <driver/serial.h>
#include <driver/vga_terminal.h>
#include <driver/keyboard.h>
#include <driver/pit.h>

#define TEST_HEAP_SIZE (1024*2)
uint8_t heap_buffer[TEST_HEAP_SIZE];
StaticHeap* kernelHeap;

#define PIT_FREQUENCY_HZ 100000

void pit_test(uint32_t ticks) {
	if(ticks % PIT_FREQUENCY_HZ == 0) {
		terminal_writestring("Heart is beating...\n");
	}
}

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
	terminal_writestring("Terminal initialized\n");

  /* Initializes serial com 1, used for debugging */
	serial_init(SERIAL_COM1_BASE);
	serial_writestring("Testing the beauty of the serial port\n");
	arch_initialize();

	pit_initialize(PIT_FREQUENCY_HZ, &pit_test);
	keyboard_initialize();
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
