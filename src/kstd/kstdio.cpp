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

void logch(char c) {
    char str[2];
    str[0] = c;
    str[1] = '\0';
    global_debug_writestring(str);
}

void puterrch(char c) {
    char str[2];
    str[0] = c;
    str[1] = '\0';
    global_error_writestring(str);
}

void putch(char c) {
    char str[2];
    str[0] = c;
    str[1] = '\0';
    global_out_writestring(str);
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