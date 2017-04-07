#ifndef KTERM_TERMINAL_H
#define KTERM_TERMINAL_H
#include <stddef.h>
#include <string.h>
#include <util/ring.h>
#include <stdint.h>

const size_t TERMINAL_BUFFER_SIZE = 0x5000; //2K

namespace kterm {
    using util::ring;

    enum TerminalMode {
        RAW,
        COOKED
    };

    class Terminal {
        private:
        ring<char> outBuffer;
        ring<char> inBuffer;
        TerminalMode mode;
        bool inputEcho;
        //Helpers
        //Draws the screen on the vga_terminal, starting with a given line offset
        void flushOutBuffer();
        public:
        Terminal();
        ~Terminal();
        void setMode(TerminalMode mode);
        TerminalMode getMode();
        void putchar(char c);
        void puts(const char* str);
        char getchar();
        kstd::string gets();
        void become_master();
        void _signal_key_pressed(unsigned char scancode);
        //kstd::string readLine();
        //void putLine(char* str);
        //void putLine(kstd::string str);
    };
    
};
#endif
