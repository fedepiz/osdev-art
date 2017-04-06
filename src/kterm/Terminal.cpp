#include <kterm/Terminal.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <driver/vga_terminal.h>
#include <util/text.h>

namespace kterm {
    using kstd::string;

    Terminal::Terminal() : lineBuffer(TERMINAL_NUM_LINES) {
        this->echoLine = true;
        this->xPos = 0;
    }

    Terminal::~Terminal() {
    }

    void Terminal::flush(unsigned int lineOffset) {
    }

    string Terminal::readLine() {

    }

    void Terminal::putLine(char* str) {
        
    }

    void Terminal::putLine(string str) {
        this->putLine(str.str());
    }
};