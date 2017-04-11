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

    unsigned char kbdus_lower[128] = {
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
//Uppercase version
unsigned char kbdus_upper[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
  '(', ')', '_', '+', '\b',	/* Backspace */
  '\t',			/* Tab */
  'Q', 'W', 'E', 'R',	/* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 39 */
 '"', '~',   0,		/* Left shift */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
  'M', '<', '>', '?',   0,				/* Right shift */
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
    //Signals to the master terminal that a key has been pressed
    //Used to hook up keyboard drivers to the terminal
    void inputMasterOnKeyPress(keyboard::key_info_t key_info) {
        if(inputMasterTerminal != nullptr) {
            inputMasterTerminal->_signal_key_pressed(key_info);
        }
    }

    //Writes to the master terminal. Main use is to pass to system-wide
    //selectors, so that things like printf will be directed to here
    void inputMasterWritestring(const char* str) {
        if(inputMasterTerminal != nullptr) {
            inputMasterTerminal->puts(str);
        }
    }

    Terminal::Terminal() {
        this->mode = TerminalMode::RAW;
        this->inputEcho = true;
        this->currentForegroundColor = defaultForegroundColor;
        this->currentBackgroundColor = defaultBackgroundColor;
    }

    Terminal::~Terminal() {
        if(inputMasterTerminal == this) {
            inputMasterTerminal = nullptr;
        }
    }

    void Terminal::setForegroundColor(VGAColor col) {
        this->currentForegroundColor = col;
    }

    void Terminal::setBackgroundColor(VGAColor col) {
        this->currentBackgroundColor = col;
    }

    VGAColor Terminal::getForegroundColor() const {
        return this->currentForegroundColor;
    }

    VGAColor Terminal::getBackgroundColor() const {
        return this->currentBackgroundColor;
    }

    void Terminal::setMode(TerminalMode mode) {
        this->flushOutBuffer();
        this->mode = mode;
    }

   TerminalMode Terminal::getMode() {
        return this->mode;
    }

    void Terminal::writechar(char c) {
        auto fg_col = (vga_term::color)this->currentForegroundColor;
        auto bg_col = (vga_term::color)this->currentBackgroundColor;
        vga_term::set_color(fg_col, bg_col);
        vga_term::putchar(c);
    }

    void Terminal::putchar(char c) {
        if(this->mode == RAW) {
            this->writechar(c);
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

    void Terminal::puts(string str) {
        this->puts(str.str());
    }
    
    void Terminal::flushOutBuffer() {
        while(!this->outBuffer.is_empty()) {
            char c = outBuffer.behead();
            this->writechar(c);
        }
    }

    bool Terminal::hasInput() {
        return this->inBuffer.is_empty();
    }
    

    char Terminal::keyToChar(key_info_t key_info) {
        unsigned char* keymap;
        if(key_info.shift_down) {
            keymap = kbdus_upper;
        } else {
            keymap = kbdus_lower;
        }
        char ch = keymap[key_info.keycode];
        return ch;
    }

    char Terminal::getchar() {
        //Block until we have input
        retry:
        while(!this->hasInput()) {
            //hacky for now, but enforces the loop
            logf("");
        }
        key_info_t key_info = this->inBuffer.behead();
        char ch = this->keyToChar(key_info);
        if(this->inputEcho && !keyboard::key_is_special(key_info.keycode)) {
            this->putchar(ch);
        } else {
            goto retry;
        }
        //logf("LEAVING GETCHAR\n");
        return ch;
    }

    string Terminal::gets() {
        
        util::vector<char> vec(10);
        
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
        delete[] c_str;
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