#ifndef KERNEL_STD
#define KERNEL_STD

#include <stddef.h>
#include <stdint.h>
size_t k_strlen(const char* str);
void k_memset(void* ptr, uint8_t val, size_t size);
void k_memcpy(void* destination, const void* source, size_t num);
int atoi(char *str);
int itoa(int value, char *sp, int radix = 10);
void k_panic(const char* message);
#endif
