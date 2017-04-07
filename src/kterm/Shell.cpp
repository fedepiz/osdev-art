#include <kterm/Shell.h>
#include <kterm/Terminal.h>
#include <string.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <util/text.h>

namespace kterm {
    using kstd::string;


    Shell::Shell(Terminal* term) {
        this->term = term;
    }

    Terminal* Shell::getTerminal() {
        return this->term;
    }

    void Shell::mainLoop() {
        bool keep_going = true;
        while(keep_going) {
            string line = this->term->gets();
            keep_going = line != string("quit");
        }
    }
};
