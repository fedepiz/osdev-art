#ifndef KERNEL_STD
#define KERNEL_STD

#include <stddef.h>
#include <stdint.h>
#include <util/DynamicHeap.h>

namespace kstd {

#define SMALL_STRING_SIZE 25

typedef struct smallString {
    char str[SMALL_STRING_SIZE];
}smallString;

size_t strlen(const char* str);
void memset(void* ptr, uint8_t val, size_t size);
void memcpy(void* destination, const void* source, size_t num);
int atoi(char *str);
int itoa(int value, char *sp, int radix = 10);
smallString itoa(int value, int radix = 10);
void puts(const char* message);
void puterr(const char* message);
void log(const char* message);


extern util::DynamicHeap kernel_heap;
void memory_initialize();
void* malloc(size_t count);
void* calloc(size_t count);
void free(void* ptr);
void cfree(void* ptr);


};

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT "\nat" __FILE__ ":" TOSTRING(__LINE__) "\n"

void _panic(const char* message, const char* pos);
#define panic(message) _panic(message, AT);
#endif
