#include <kernel/arch.h>
#include <kernel/globals.h>
#include <kernel/kstd.h>
#include <kernel/frame_alloc.h>
#include <kernel/paging.h>
#include <util/StaticHeap.h>
#include <driver/serial.h>
#include <driver/vga_terminal.h>
#include <driver/keyboard.h>
#include <driver/pit.h>

#define TEST_HEAP_SIZE (1024*2)
uint8_t heap_buffer[TEST_HEAP_SIZE];
StaticHeap* kernelHeap;

using kstd::log;
using kstd::itoa;


void pit_test(uint32_t ticks) {
	if(ticks % pit::DEFAULT_FREQUENCY_HZ == 0) {
		kstd::puts("Heart is beating...\n");
	}
}

void initialize() {
	/* Initialize the global function dispatch selector */
	struct globals globals;
	globals.error_writestring = &vga_term::puts;
	globals.out_writestring = &vga_term::puts;
	globals.debug_writestring = &serial::puts;
	/* Complete global initialization */
	globals_initialize(globals);
	/* Initialize the kernel heap */
	//k_heap_initialize();
	/* Initialize terminal interface */
	vga_term::initialize();
	vga_term::puts("Terminal initialized\n");

  /* Initializes serial com 1, used for debugging */
	serial::initialize(SERIAL_COM1_BASE);
	serial::puts("Testing the beauty of the serial port\n");
	
	arch::initialize();
	frame_alloc_initialize();
	paging::initialize();

	pit::initialize(pit::DEFAULT_FREQUENCY_HZ, &pit_test);
	keyboard::initialize();
}


void hang() {
	log("Hanging...\n");
	for(;;) {

	}
}

void testStaticHeap() {
	log("Testing static heap\n");
	StaticHeap lclHeap(heap_buffer, TEST_HEAP_SIZE);
	lclHeap.setStrict(true);
	log("INITIAL HEAP STATE\n");
	lclHeap.debug();
	auto* array1 = (uint32_t*)lclHeap.malloc(sizeof(uint32_t)*4);
	auto* array2 = (uint32_t*)lclHeap.malloc(sizeof(uint32_t)*4);
	array1[0] = 52;
	array2[0] = 24;
	log("This should be 52: ");
	log(itoa(array1[0]).str);
	log("\n");
	heapSize heapSize = lclHeap.getSize();
	log("Free size: ");
	log(itoa(heapSize.free).str);
	log("\nAllocated size: ");
	log(itoa(heapSize.allocated).str);
	log("\nAFTER ALLOCATIONS\n");
	lclHeap.debug();
	lclHeap.free(array2);
	log("AFTER ONE FREE\n");
	lclHeap.debug();
	lclHeap.free(array1);
	log("AFTER TWO FREES\n");
	lclHeap.debug();
}


extern "C" void kernel_main(void) {
	initialize();
	paging::debug();
	//testStaticHeap();
	vga_term::puts("Hello, kernel World!\n");
	hang();
}
