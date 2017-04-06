#include <kterm/Terminal.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <driver/vga_terminal.h>

namespace kterm {
    using kstd::string;

    Terminal::Terminal() {
        //Zero the buffer
        kstd::memset(this->buffer, 0, TERMINAL_BUFFER_SIZE);
        //head offset is 0
        this->bufferHead = this->buffer;
        this->echoLine = true;
    }

    Terminal::~Terminal() {
    }

    string Terminal::readLine() {

    }

    void Terminal::putLine(char* str) {
        
    }

    void Terminal::putLine(string str) {
        this->putLine(str.str());
    }
};