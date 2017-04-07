#include <kterm/Terminal.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <kernel/globals.h>
#include <driver/vga_terminal.h>
#include <driver/keyboard.h>
#include <util/text.h>
#include <util/vector.h>

namespace kterm {
    using kstd::string;
    using util::logf;
    using keyboard::key_info_t;

    unsigned char kbdus[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};		

    Terminal* inputMasterTerminal = nullptr;
    void inputMasterOnKeyPress(keyboard::key_info_t key_info) {
        if(inputMasterTerminal != nullptr) {
            inputMasterTerminal->_signal_key_pressed(key_info);
        }
    }

    void inputMasterWritestring(const char* str) {
        if(inputMasterTerminal != nullptr) {
            inputMasterTerminal->puts(str);
        }
    }

    Terminal::Terminal() {
        this->mode = TerminalMode::RAW;
        this->inputEcho = true;
    }

    Terminal::~Terminal() {
        if(inputMasterTerminal == this) {
            inputMasterTerminal = nullptr;
        }
    }

    void Terminal::setMode(TerminalMode mode) {
        this->flushOutBuffer();
        this->mode = mode;
    }

   TerminalMode Terminal::getMode() {
        return this->mode;
    }

    void Terminal::putchar(char c) {
        if(this->mode == RAW) {
            vga_term::putchar(c);
        } else if(this->mode == COOKED) {
            this->outBuffer.append(c);
            //User pressed enter
            if(c == '\n') {
                this->flushOutBuffer();
            }
        }
    }

    void Terminal::puts(const char* str) {
        for(size_t i = 0; i < kstd::strlen(str);i++) {
            this->putchar(str[i]);
        }
    }
    
    void Terminal::flushOutBuffer() {
        while(!this->outBuffer.is_empty()) {
            char c = outBuffer.behead();
            vga_term::putchar(c);
        }
    }

    bool Terminal::hasInput() {
        return this->inBuffer.is_empty();
    }
    

    char Terminal::keyToChar(key_info_t key_info) {
        char ch = kbdus[key_info.keycode];
        if(key_info.shift_down) {
            if(kstd::isalpha(ch)) ch = kstd::toupper(ch);
            if(kstd::issymbol(ch)) ch = ch + 16;
        }
        return ch;
    }

    char Terminal::getchar() {
        //logf("ENTERING GETCHAR\n");
        //Block until we have input
        while(!this->hasInput()) {
            //hacky for now, but enforces the loop
            logf("");
        }

        key_info_t key_info = this->inBuffer.behead();
        char ch = this->keyToChar(key_info);

        if(this->inputEcho && !keyboard::key_is_special(key_info.keycode)) {
           this->putchar(ch);
        }
        //logf("LEAVING GETCHAR\n");
        return ch;
    }

    string Terminal::gets() {
        //logf("ENTERING GETS\n");
        util::vector<char> vec;
        char c = this->getchar();
        //While not enter
        while(c != '\n') {
            vec.push_back(c);
            c = this->getchar();
        }
        //char* c_str = util::vector_as_string(vec);
        char* c_str = new char[vec.size()+1];
        for(unsigned int i = 0; i < vec.size(); i++) {
            c_str[i] = vec[i];
        }
        c_str[vec.size()] = '\0';

        string str(c_str);
        //delete c_str;
        //logf("LEAVING GETS\n");
        return str;
    }

    void Terminal::become_master() {
        inputMasterTerminal = this;
        keyboard::active_key_handler = inputMasterOnKeyPress;
        globals* globs = get_globals();
        globs->out_writestring = inputMasterWritestring;
        globs->error_writestring = inputMasterWritestring;
    }

    void Terminal::_signal_key_pressed(key_info_t key_info) {
        if(key_info.released == true) {
            this->inBuffer.append(key_info);
        }
    }
};