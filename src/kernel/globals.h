#ifndef KERNEL_GLOBALS_H
#define KERNEL_GLOBALS_H

typedef void (*writestring_f)(const char* str);

struct globals {
    writestring_f error_writestring;
    writestring_f out_writestring;
    writestring_f debug_writestring;
};

void globals_initialize(struct globals);
struct globals* get_globals();

void global_error_writestring(const char* str);
void global_out_writestring(const char* str);
void global_debug_writestring(const char* str);
#endif