#ifndef UTIL_TEXT_H
#define UTIL_TEXT_H
#include <stdarg.h>
namespace util {
    char* vsformat(const char* fmt, va_list args);
    char* format(const char* fmt, ...);

    void printf(const char* fmt, ...);
    void logf(const char* fmt, ...);
    void panicf(const char* fmt, ...);
};
#endif