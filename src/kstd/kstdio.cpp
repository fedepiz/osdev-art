#include <kernel/globals.h>

namespace kstd {

void puts(const char* message) {
    global_out_writestring(message);
}

void puterr(const char* message) {
    global_error_writestring(message);
}

void log(const char* message) {
    global_debug_writestring(message);
}
};

void _panic(const char* message, const char* pos) {
    kstd::puterr("PANIC: ");
	kstd::puterr(message);
    kstd::puterr(pos);
	for(;;) {
		//Hang
	}
}