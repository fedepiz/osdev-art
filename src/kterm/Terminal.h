#ifndef KTERM_TERMINAL_H
#define KTERM_TERMINAL_H
#include <stddef.h>
#include <string.h>
#include <util/ring.h>
#include <stdint.h>
namespace kterm {
    using util::ring;

    const size_t TERMINAL_LINE_LENGTH = 80;
    const size_t TERMINAL_NUM_LINES = 25;
    const size_t TERMINAL_BUFFER_SIZE = TERMINAL_LINE_LENGTH*TERMINAL_NUM_LINES*15;

    struct term_line {
        char str[TERMINAL_LINE_LENGTH];
    };

    class Terminal {
        private:
        ring<term_line> lineBuffer;
        bool echoLine;
        int xPos;
        //Helpers
        //Draws the screen on the vga_terminal, starting with a given line offset
        void flush(unsigned int lineOffset);
        public:
        Terminal();
        ~Terminal();
        kstd::string readLine();
        void putLine(char* str);
        void putLine(kstd::string str);
    };
    
};
#endif
