#ifndef KTERM_SHELL_H
#define KTERM_SHELL_H

#include <kterm/Terminal.h>
#include <string.h>


namespace kterm {
    class Shell {
        private:
        Terminal* term;
        void processCommand(const kstd::string& line);
        public:
        Shell(Terminal* term);
        Terminal* getTerminal();
        void mainLoop();
    };
};
#endif