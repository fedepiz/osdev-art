#include <driver/vga_terminal.h>
#include <kstdlib.h>
#include <kernel/arch.h>

namespace vga_term {
const uint32_t TERMINAL_BUFFER_BASE_ADDRESS = 0x000B8000;

bool wrap_around = false;

static inline uint8_t vga_entry_color(enum color fg, enum color bg) {
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

void set_wrap_around(bool state){
	wrap_around = state;
}

void set_color(uint8_t color) {
	terminal_color = color;
}

void set_color(color fg, color bg) {
	set_color(vga_entry_color(fg, bg));
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

static void move_cursor_to(unsigned int x, unsigned int y) {
	unsigned int linear = y * VGA_WIDTH + x;

	arch::outb(0x3D4, 14);
    arch::outb(0x3D5, linear >> 8);
    arch::outb(0x3D4, 15);
    arch::outb(0x3D5, linear);
}

static void copy_up() {
	for(size_t i = 0; i < VGA_HEIGHT - 1; i++) {
		uint16_t* destLine = terminal_buffer + i*VGA_WIDTH;
		uint16_t* sourceLine = terminal_buffer + (i + 1)*VGA_WIDTH;
		kstd::memcpy(destLine, sourceLine, sizeof(uint16_t)*VGA_WIDTH);
	}
	kstd::memset(terminal_buffer + VGA_WIDTH*(VGA_HEIGHT -1),0,sizeof(uint16_t)*VGA_WIDTH);
}

static void advance_row() {
  if (++terminal_row == VGA_HEIGHT) {
		if(wrap_around) {
    	terminal_row = 0;
		} else {
			--terminal_row;
			//copy everything up
			copy_up();
		}
  }
}

static void advance_column() {
  if(++terminal_column == VGA_WIDTH) {
    terminal_column = 0;
    advance_row();
  }
}

static void back() {
	if(terminal_column > 0) {
		//Go back
		terminal_column--;
		//Write black
		putchar(' ');
		//and back again
		terminal_column--;
	}
}

 void putchar(char c) {
  if(c == '\n') {
    terminal_column = 0;
    advance_row();
  } else if (c == '\b') {
	  back();
  }else {
	  terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	  advance_column();
  }
 move_cursor_to(terminal_column, terminal_row);
}

void write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		putchar(data[i]);
}

void puts(const char* data) {
	write(data, kstd::strlen(data));
}

void clear() {
	kstd::memset(terminal_buffer, 0, VGA_WIDTH*VGA_HEIGHT*sizeof(uint16_t));
	terminal_column = 0;
	terminal_row = 0;
}
};