#include <kernel/globals.h>
struct globals currentGlobals;

void globals_initialize(struct globals glbs) {
    currentGlobals = glbs;
}
struct globals* get_globals() {
    return &currentGlobals;
}

void global_error_writestring(const char* str) {
    currentGlobals.error_writestring(str);
}

void global_out_writestring(const char* str) {
    currentGlobals.out_writestring(str);
}

void global_debug_writestring(const char* str) {
    currentGlobals.debug_writestring(str);
}