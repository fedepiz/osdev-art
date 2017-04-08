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
	//kterm::Terminal terminal;
	//terminal.become_master();

	kterm::Shell shell(nullptr);
	shell.mainLoop();
	hang();
}
