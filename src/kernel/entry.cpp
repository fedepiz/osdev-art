#include <kstdio.h>
#include <kstdlib.h>
#include <kernel/arch.h>
#include <kernel/globals.h>
#include <kernel/frame_alloc.h>
#include <kernel/paging.h>
#include <kernel/multiboot.h>
#include <util/StaticHeap.h>
#include <util/DynamicHeap.h>
#include <driver/serial.h>
#include <driver/vga_terminal.h>
#include <driver/keyboard.h>
#include <driver/pit.h>
#include <filesystem/VFS.h>
#include <util/vector.h>
#include <util/text.h>
using kstd::log;
using kstd::itoa;
using kstd::malloc;
using kstd::free;

using util::logf;


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
	uint32_t mem_size = mbinfo->mem_upper*1024; //1 KiB per unit
	frame_alloc::initialize(arch::kernel_size(), mem_size);
	//Reserve the memory of the multiboot modules
	//(must happen before we initialize the kernel heap and start dishing out frames)
	multiboot::reserve_modules_frames(mbinfo);
	//let's now get the maximum mem-higher size
	//Initialize the paging system and kernel page allocator
	paging::initialize();
	//Create the kernel heap
	kstd::kernel_heap_initialize();
	//Core device drivers
	pit::initialize(pit::DEFAULT_FREQUENCY_HZ, &pit_test);
	keyboard::initialize();

	vfs::initialize();
	vfs::mount_multiboot_modules(mbinfo);
	//util::logf("%s\n", vfs::root->getName());
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

void print_rainbow(const char* ptr) {
	uint8_t color = 0;
	uint8_t colorbg = 6;
	while(*ptr != '\0') {
		uint8_t actualColor = color;
		uint8_t actualBgColor = colorbg;
		if(actualColor == actualBgColor) {
			actualColor = 15;
			actualBgColor = 0;
		}
		vga_term::set_color(static_cast<vga_term::color>(actualColor), static_cast<vga_term::color>(actualBgColor));
		vga_term::putchar(*ptr);
		color++;
		colorbg--;
		if(color > 6) color = 0;
		if(colorbg == 0) colorbg = 6;
		ptr++;
	}
	vga_term::set_color(vga_term::color::VGA_COLOR_WHITE);
}


/*
	#include <util/ring.h>
	ring test code
	util::ring<int> ring(5);
	ring.push_back(1);
	ring.push_back(2);
	ring.push_back(3);
	logf("%d %d %d\n", ring[0], ring[1], ring[2]);
	ring.pop_front();
	logf("%d %d\n", ring[0], ring[1]);
	ring.pop_back();
	logf("%d %d\n", ring.count(), ring.available());
	ring.push_back(0);
	ring.push_back(0);
	ring.push_back(0);
	ring.push_back(0);
	logf("%d %d\n", ring.count(), ring.available());
	ring.push_back(0);
*/
#include <kterm/Terminal.h>
extern "C" void kernel_main(uint32_t ebx) {
	initialize(ebx);
	//testPageAllocator();
	//testDynamicHeap();
	util::printf("Welcome to Art v0.01a\n");
	print_rainbow("\"Beauty lies in the eye of the beholder\"\n");
	
	//for(int i = 0; i < 100; i++){
	//	util::printf("%d\n", i);
	//}
	kterm::Terminal terminal;
	terminal.become_master();
	terminal.gets();
	//terminal.putLine("Test");
	//terminal.putLine("It is a clear, clear test");
	hang();
}
