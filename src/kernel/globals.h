#ifndef KERNEL_GLOBALS_H
#define KERNEL_GLOBALS_H

typedef void (*writestring_f)(const char* str);

struct globals {
    writestring_f error_writestring;
};

void globals_initialize(struct globals);
struct globals* get_globals();

void global_error_writestring(const char* str);
#endif