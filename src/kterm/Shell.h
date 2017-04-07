#ifndef KTERM_SHELL_H
#define KTERM_SHELL_H

#include <kterm/Terminal.h>

namespace kterm {
    class Shell {
        private:
        Terminal* term;
        public:
        Shell(Terminal* term);
        Terminal* getTerminal();
        void mainLoop();
    };
};
#endif