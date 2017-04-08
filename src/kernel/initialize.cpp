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

	//Filesystem
	vfs::initialize();
	vfs::mount_multiboot_modules(mbinfo);
	//util::logf("%s\n", vfs::root->getName());
}