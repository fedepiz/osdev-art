#ifndef KERNEL_STDLIB_H
#define KERNEL_STDLIB_H

#include <stdint.h>
#include <stddef.h>
#include <util/DynamicHeap.h>

namespace kstd {

#define SMALL_STRING_SIZE 25

typedef struct smallString {
    char str[SMALL_STRING_SIZE];
}smallString;

int atoi(char *str);
int itoa(int value, char *sp, int radix = 10);
smallString itoa(int value, int radix = 10);

size_t strlen(const char* str);
int strcmp(const char* str1,const char* str2);

void memset(void* ptr, uint8_t val, size_t size);
void memcpy(void* destination, const void* source, size_t num);

extern util::DynamicHeap kernel_heap;
void kernel_heap_initialize();

void* malloc(size_t count);
void* calloc(size_t count);
void free(void* ptr);
void cfree(void* ptr);

};
#endif