#ifndef KERNEL_STD
#define KERNEL_STD

#include <stddef.h>
#include <stdint.h>

#define SMALL_STRING_SIZE 25

typedef struct smallString {
    char str[SMALL_STRING_SIZE];
}smallString;

size_t k_strlen(const char* str);
void k_memset(void* ptr, uint8_t val, size_t size);
void k_memcpy(void* destination, const void* source, size_t num);
int atoi(char *str);
int itoa(int value, char *sp, int radix = 10);
smallString itoa(int value, int radix = 10);
void k_panic(const char* message);
void k_writestring(const char* message);
void k_error_writestring(const char* message);
void k_debug_writestring(const char* message);


#endif
