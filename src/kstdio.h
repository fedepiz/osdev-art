#ifndef KERNEL_STDIO_H
#define KERNEL_STDIO_H

#include <stddef.h>
#include <stdint.h>

namespace kstd {


void puts(const char* message);
void puterr(const char* message);
void log(const char* message);
void putch(char c);
void logch(char c);
void puterrch(char c);
};


#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT "\nat" __FILE__ ":" TOSTRING(__LINE__) "\n"

void _panic(const char* message, const char* pos);
#define panic(message) _panic(message, AT);
#endif