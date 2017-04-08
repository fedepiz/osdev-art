#include <kterm/Terminal.h>
#include <kterm/Shell.h>
#include <util/text.h>
#include <kernel/initialize.h>
#include <driver/vga_terminal.h>

using util::logf;

void hang() {
	logf("Hanging...\n");
	for(;;) {

	}
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



extern "C" void kernel_main(uint32_t ebx) {
	initialize(ebx);
	//testPageAllocator();
	//testDynamicHeap();
	util::printf("Welcome to Art v0.01a\n");
	print_rainbow("\"Beauty lies in the eye of the beholder\"\n");

	/*
	util::list<int> list;
	list.append(3);
	list.append(4);
	list.append(5);
	logf("List length %d ( = 3)\n", list.length());
	int head = list.behead();
	logf("After beheading, value is %d, length %d\n", head, list.length());
	*/

	//kstd::kernel_heap.chatty_mode(true);
		
	//Creates a terminal and makes it the main input controller	
	kterm::Terminal terminal;
	terminal.become_master();

	kterm::Shell shell(&terminal);
	shell.mainLoop();
	hang();
}
