#ifndef KTERM_TERMINAL_H
#define KTERM_TERMINAL_H
#include <stddef.h>
#include <string.h>
#include <driver/keyboard.h>
#include <util/list.h>
#include <stdint.h>

const size_t TERMINAL_BUFFER_SIZE = 0x5000; //2K

namespace kterm {
    using util::list;

    enum TerminalMode {
        RAW,
        COOKED
    };

    class Terminal {
        private:
        list<char> outBuffer;
        list<keyboard::key_info_t> inBuffer;
        TerminalMode mode;
        bool inputEcho;
        //Helpers
        //Draws the screen on the vga_terminal, starting with a given line offset
        void flushOutBuffer();
        bool hasInput();
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
        void _signal_key_pressed(keyboard::key_info_t key_info);
        //kstd::string readLine();
        //void putLine(char* str);
        //void putLine(kstd::string str);
    };
    
};
#endif
