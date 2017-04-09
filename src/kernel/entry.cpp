#include <kterm/Terminal.h>
#include <kterm/Shell.h>
#include <util/text.h>
#include <kernel/initialize.h>

using util::logf;

void hang() {
	logf("Hanging...\n");
	for(;;) {

	}
}

extern "C" void kernel_main(uint32_t ebx) {
	initialize(ebx);
	//kstd::kernel_heap.chatty_mode(true);
		
	//Creates a terminal and makes it the main input controller	
	logf("Initialization done\n");
	kterm::Terminal terminal;
	terminal.become_master();
	logf("Terminal launched\n");

	kterm::Shell shell(&terminal);
	logf("Shell operative\n");
	shell.mainLoop();
	hang();
}
