#include <driver/vga_terminal.h>
#include <kstdlib.h>
#include <kernel/arch.h>

namespace vga_term {
const uint32_t TERMINAL_BUFFER_BASE_ADDRESS = 0x000B8000;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;


size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) (TERMINAL_BUFFER_BASE_ADDRESS + arch::KERNEL_VIRTUAL_BASE);
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void advance_row() {
  if (++terminal_row == VGA_HEIGHT) {
    terminal_row = 0;
  }
}

void advance_column() {
  if(++terminal_column == VGA_WIDTH) {
    terminal_column = 0;
    advance_row();
  }
}

void putchar(char c) {
  if(c == '\n') {
    terminal_column = 0;
    advance_row();
  } else {
	  terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	  advance_column();
  }
}

void write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		putchar(data[i]);
}

void puts(const char* data) {
	write(data, kstd::strlen(data));
}
};