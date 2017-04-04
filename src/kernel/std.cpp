#include <kernel/globals.h>
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

void k_memcpy(void* destination, const void* source, size_t num) {
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
    k_memset(str.str, 0, SMALL_STRING_SIZE);
    itoa(value, str.str, radix);
    return str;
}

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT "\nat" __FILE__ ":" TOSTRING(__LINE__) "\n"

void k_panic(const char* message) {
	global_error_writestring("PANIC: ");
	global_error_writestring(message);
	global_error_writestring(AT);
	for(;;) {
		//Hang
	}
}

void k_writestring(const char* message) {
    global_out_writestring(message);
}

void k_error_writestring(const char* message) {
    global_error_writestring(message);
}

void k_debug_writestring(const char* message) {
    global_debug_writestring(message);
}
