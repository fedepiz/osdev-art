#ifndef KERNEL_STD
#define KERNEL_STD

#include <stddef.h>
#include <stdint.h>
size_t k_strlen(const char* str);
void k_memset(void* ptr, uint8_t val, size_t size);
#endif
