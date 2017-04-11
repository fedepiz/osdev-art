#ifndef KTERM_TERMINAL_H
#define KTERM_TERMINAL_H

#include <stddef.h>
#include <string.h>
#include <driver/keyboard.h>
#include <util/list.h>
#include <stdint.h>
#include <string.h>

const size_t TERMINAL_BUFFER_SIZE = 0x5000; //2K

namespace kterm {
    using util::list;

    enum TerminalMode {
        RAW,
        COOKED
    };

    enum VGAColor {
        BLACK = 0,
        BLUE = 1,
        GREEN = 2,
        CYAN = 3,
        RED = 4,
        MAGENTA = 5,
        BROWN = 6,
        LIGHT_GREY = 7,
        DARK_GREY = 8,
        LIGHT_BLUE = 9,
        LIGHT_GREEN = 10,
        LIGHT_CYAN = 11,
        LIGHT_RED = 12,
        LIGHT_MAGENTA = 13,
        LIGHT_BROWN = 14,
        WHITE = 15,
    };

    const VGAColor defaultForegroundColor = VGAColor::WHITE;
    const VGAColor defaultBackgroundColor = VGAColor::BLACK;

    class Terminal {
        private:
        list<char> outBuffer;
        list<keyboard::key_info_t> inBuffer;
        TerminalMode mode;
        VGAColor currentForegroundColor;
        VGAColor currentBackgroundColor;
        bool inputEcho;
        //Helpers
        //Draws the screen on the vga_terminal, starting with a given line offset
        void writechar(char c);
        void flushOutBuffer();
        char keyToChar(keyboard::key_info_t key_info);
        void waitForInput();
        public:
        Terminal();
        ~Terminal();
        //Color set
        void setForegroundColor(VGAColor col);
        void setBackgroundColor(VGAColor col);
        VGAColor getForegroundColor() const;
        VGAColor getBackgroundColor() const;
        //Terminal modes control
        void setMode(TerminalMode mode);
        TerminalMode getMode();
        //Sends data to the terminal for output
        void putchar(char c);
        void puts(const char* str);
        void puts(kstd::string str);
        //Asks the terminal to produce input
        bool hasInput();
        char getchar();
        kstd::string gets();
        //Interaction with system-wide buffers
        void become_master();
        void _signal_key_pressed(keyboard::key_info_t key_info);
    };
};
#endif
