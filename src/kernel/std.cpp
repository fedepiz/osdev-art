#include <kernel/std.h>

size_t k_strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void k_memset(void* ptr, uint8_t val, size_t size) {
	uint8_t* data = (uint8_t*)ptr;
	for(size_t i = 0; i < size; i++) {
		data[i] = val;
	}
}
