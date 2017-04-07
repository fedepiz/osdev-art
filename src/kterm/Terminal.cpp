#include <kterm/Terminal.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <driver/vga_terminal.h>
#include <driver/keyboard.h>
#include <util/text.h>
#include <util/vector.h>

namespace kterm {
    using kstd::string;
    using util::logf;

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
    void inputMasterOnKeyPress(unsigned char keycode) {
        if(inputMasterTerminal != nullptr) {
            inputMasterTerminal->_signal_key_pressed(keycode);
        }
    }

    Terminal::Terminal() : outBuffer(TERMINAL_BUFFER_SIZE), inBuffer(TERMINAL_BUFFER_SIZE) {
        this->mode = TerminalMode::RAW;
        this->inputEcho = true;
    }

    Terminal::~Terminal() {
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
            this->outBuffer.push_back(c);
            //User pressed enter
            if(c == '\n') {
                this->flushOutBuffer();
            }
        }
    }

    void Terminal::puts(char* str) {
        for(size_t i = 0; i < kstd::strlen(str);i++) {
            this->putchar(str[i]);
        }
    }
    
    void Terminal::flushOutBuffer() {
        while(this->outBuffer.count() > 0) {
            char c = outBuffer.pop_front();
            vga_term::putchar(c);
        }
    }

    char Terminal::getchar() {
        char ch;
        if(inBuffer.count() > 0) {
            ch = inBuffer.pop_front();
        } else {
            //Must do block read - unregister if currently the master
            bool isMaster = this == inputMasterTerminal;
            if(isMaster) {
                inputMasterTerminal = nullptr;
            }
            //Invoke blocking keyboard driver read
            unsigned char scancode = keyboard::get_key();
            //Re-activate master if was so
            if(isMaster) {
              inputMasterTerminal = this;
            }
            
            ch = kbdus[scancode];
        }
        if(this->inputEcho) {
           this->putchar(ch);
        }
        return ch;
    }

    string Terminal::gets() {
        util::vector<char> vec;
        char c = this->getchar();
        //While not enter
        while(c != '\n') {
            vec.push_back(c);
            c = this->getchar();
        }
        char* c_str = util::vector_as_string(vec);
        string str(c_str);
        delete c_str;
        return str;
    }

    void Terminal::become_master() {
        inputMasterTerminal = this;
        keyboard::active_on_key_press_handler = inputMasterOnKeyPress;
    }

    void Terminal::_signal_key_pressed(unsigned char scancode) {
        this->inBuffer.push_back(scancode);
    }
    /*
    string Terminal::readLine() {
    }

    void Terminal::putLine(char* str) {
        
    }

    void Terminal::putLine(string str) {
        this->putLine(str.str());
    } */
};