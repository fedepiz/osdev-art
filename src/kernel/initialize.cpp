#include <kstdio.h>
#include <kstdlib.h>
#include <kernel/arch.h>
#include <kernel/globals.h>
#include <kernel/multiboot.h>
#include <util/StaticHeap.h>
#include <memory/subsystem.h>
#include <memory/paging.h>
#include <driver/serial.h>
#include <driver/vga_terminal.h>
#include <driver/keyboard.h>
#include <driver/pit.h>
#include <filesystem/VFS.h>
#include <util/vector.h>
#include <util/text.h>
#include <string.h>
using kstd::log;
using kstd::itoa;
using kstd::string;
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

	memory::initialize(mbinfo);
	pit::initialize(pit::DEFAULT_FREQUENCY_HZ, &pit_test);
	keyboard::initialize();

	//Filesystem
	vfs::VFSNode* root = vfs::initialize();
	vfs::mount_modules(root, mbinfo);
	//util::logf("%s\n", vfs::root->getName());
}