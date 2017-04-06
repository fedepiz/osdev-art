#include <kernel/arch.h>
#include <kernel/globals.h>
#include <kernel/kstd.h>
#include <kernel/frame_alloc.h>
#include <kernel/paging.h>
#include <kernel/multiboot.h>
#include <util/StaticHeap.h>
#include <util/DynamicHeap.h>
#include <driver/serial.h>
#include <driver/vga_terminal.h>
#include <driver/keyboard.h>
#include <driver/pit.h>

using kstd::log;
using kstd::itoa;
using kstd::malloc;
using kstd::free;


void pit_test(uint32_t ticks) {
	(void)ticks;
	//if(ticks % pit::DEFAULT_FREQUENCY_HZ == 0) {
	//	kstd::puts("Heart is beating...\n");
	//}
}

void initialize(uint32_t ebx) {
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
	//vga_term::puts("Terminal initialized\n");

  /* Initializes serial com 1, used for debugging */
	serial::initialize(SERIAL_COM1_BASE);
	//serial::puts("Testing the beauty of the serial port\n");

	//Load the multiboot header
	auto mbinfo = (multiboot::multiboot_info_t*)(ebx + arch::KERNEL_VIRTUAL_BASE);

	//Initialize the basic architecture parts (gdt, idt, interrupts, etc etc)
	arch::initialize();
	//Initialize the kernel frame allocator
	frame_alloc::initialize(arch::kernel_size());
	//Reserve the memory of the multiboot modules
	//(must happen before we initialize the kernel heap and start dishing out frames)
	multiboot::reserve_modules_frames(mbinfo);
	//Initialize the paging system and kernel page allocator
	paging::initialize();
	//Create the kernel heap
	kstd::kernel_heap_initialize();

	//Core device drivers
	pit::initialize(pit::DEFAULT_FREQUENCY_HZ, &pit_test);
	keyboard::initialize();
}


void hang() {
	log("Hanging...\n");
	for(;;) {

	}
}


void testPageAllocator() {
	paging::page_allocator* alloc = &paging::kernel_page_allocator;
	alloc->debug(true);
	uint8_t* ptr = (uint8_t*)paging::page_index_to_address(alloc->allocate());
	vga_term::puts("Base address of page ");
	vga_term::puts(itoa((uint32_t)ptr,16).str);
	vga_term::puts("\n");
	int* x = (int*)ptr;
	int* y = (int*)ptr;
	*x = 5;
	vga_term::puts(itoa(*x).str);
	vga_term::puts("\n");
	vga_term::puts(itoa(*y).str);
	vga_term::puts("\n");
}

void testDynamicHeap() {
	util::DynamicHeap heap;
	util::DynamicHeap_initialize(&heap, &paging::kernel_page_allocator);

	int* ptr[4];
	for(int i = 0; i < 4; i++) {
		ptr[i] = (int*)heap.malloc(63);
	}
	heap.free(ptr[2]);
	heap.free(ptr[1]);
	paging::kernel_page_allocator.debug(true);
	heap.debug();
}

#include <util/vector.h>
#include <util/text.h>

extern "C" void kernel_main(uint32_t ebx) {
	initialize(ebx);
	//testPageAllocator();
	//testDynamicHeap();

	util::printf("Welcome to Art v0.01a\n");
	util::printf("\"Beauty lies in the eye of the beholder\"\n");
	hang();
}
