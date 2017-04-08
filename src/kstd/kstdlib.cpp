#include <kstdlib.h>
#include <memory/MemoryAllocator.h>
#include <memory/subsystem.h>

namespace kstd {
size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

int strcmp(const char* str1, const char* str2) {
    while(*str1 != '\0') {
        char diff = *str1 - *str2;
        if(diff != 0) {
            return diff;
        }
        str1++;
        str2++;
    }
    return *str2 == '\0';
}

void memset(void* ptr, uint8_t val, size_t size) {
	uint8_t* data = (uint8_t*)ptr;
	for(size_t i = 0; i < size; i++) {
		data[i] = val;
	}
}

void memcpy(void* destination, const void* source, size_t num) {
	uint8_t* dest = (uint8_t*)destination;
	const uint8_t* src = (const uint8_t*)source;
	for(size_t i = 0; i < num; i++) {
		dest[i] = src[i];
	}
}

int atoi(char *str) {
	int res = 0; // Initialize result
  
    // Iterate through all characters of input string and
    // update result
    for (int i = 0; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';
  
    // return result.
    return res;
}

// Yet, another good itoa implementation
// returns: the length of the number string
int itoa(int value, char *sp, int radix) {
    char tmp[16];// be careful with the length of the buffer
    char *tp = tmp;
    int i;
    unsigned v;

    int sign = (radix == 10 && value < 0);    
    if (sign)
        v = -value;
    else
        v = (unsigned)value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix; // v/=radix uses less CPU clocks than v=v/radix does
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'a' - 10;
    }

    int len = tp - tmp;

    if (sign) 
    {
        *sp++ = '-';
        len++;
    }

    while (tp > tmp)
        *sp++ = *--tp;

    return len;
}

smallString itoa(int value, int radix) {
    smallString str;
    memset(str.str, 0, SMALL_STRING_SIZE);
    itoa(value, str.str, radix);
    return str;
}

void* malloc(size_t count) {
    return memory::getKernelHeap()->malloc(count);
}
void free(void* ptr) {
    memory::getKernelHeap()->free(ptr);
}

void cfree(void* ptr) {
    memory::getKernelHeap()->free(ptr);
}

void* calloc(size_t count) {
    void* ptr = memory::getKernelHeap()->malloc(count);
    memset(ptr, 0, count);
    return ptr;
}


bool isdigit(char c) {
    return c >='0' && c<='9';
}
bool isalpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z');
}
bool isalphanum(char c) {
    return isdigit(c) || isalpha(c);
}

bool issymbol(char c) {
    return (c >= '!' && c <= '/') ||
           (c >= ':' && c <= '@') ||
           (c >= '[' && c <= '`') ||
           (c >= '{' && c <= '~');
}
bool isvisible(char c) {
    return isdigit(c) || isalphanum(c) || issymbol(c);
}

char toupper(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 32;
    } else return c;
}

char tolower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    } else return c;
}
};
