#ifndef UTIL_TEXT_H
#define UTIL_TEXT_H
#include <stdarg.h>
#include <util/vector.h>
namespace util {
    char* vsformat(const char* fmt, va_list args);
    char* format(const char* fmt, ...);

    void printf(const char* fmt, ...);
    void logf(const char* fmt, ...);
    void panicf(const char* fmt, ...);

    char* vector_as_string(util::vector<char>& vec);
};
#endif