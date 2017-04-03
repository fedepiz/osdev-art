#include <kernel/std.h>

size_t k_strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
