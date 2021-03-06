#ifndef DRIVER_VGA_TERMINAL_H
#define DRIVER_VGA_TERMINAL_H
#include <stddef.h>
#include <stdint.h>

namespace vga_term {
/* Hardware text mode color constants. */
enum color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

void initialize(void);
void clear();
void set_wrap_around(bool);
void set_color(uint8_t color);
void set_color(color fg, color bg);
void putchar(char c);
void puts(const char* data);
};
#endif
