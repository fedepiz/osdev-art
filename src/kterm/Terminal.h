#ifndef KTERM_TERMINAL_H
#define KTERM_TERMINAL_H
#include <stddef.h>
#include <string.h>
#include <util/ring.h>
namespace kterm {
    const size_t TERMINAL_COLUMN_LENGTH = 80;
    const size_t TERMINAL_NUM_COLS = 25;
    const size_t TERMINAL_BUFFER_SIZE = TERMINAL_COLUMN_LENGTH*TERMINAL_NUM_COLS*15;

    class Terminal {
        private:
        char buffer[TERMINAL_BUFFER_SIZE];
        char* bufferHead;
        bool echoLine;
        //Helpers
        public:
        Terminal();
        ~Terminal();
        kstd::string readLine();
        void putLine(char* str);
        void putLine(kstd::string str);
    };
    
};
#endif
