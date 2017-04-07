#include <driver/keyboard.h>
#include <kernel/arch.h>
namespace keyboard {

using arch::inb;

//Handler function pointers
keyevent_handler_t active_on_key_press_handler = nullptr;
keyevent_handler_t active_on_key_release_handler = nullptr;

volatile bool kb_req;
volatile bool kb_block;
unsigned char kb_last_press;
bool kb_status[128];

void keyboard_handler(struct arch::regs *r) {
    (void)r;
    unsigned char scancode;

    /* Read from the keyboard's data buffer */
    scancode = inb(0x60);
    

    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    if (scancode & 0x80) {
        /* You can use this one to see if the user released the
        *  shift, alt, or control keys... */
        kb_status[scancode & (!0x80)] = false;
        if(active_on_key_release_handler != nullptr) {
            active_on_key_release_handler(scancode);
        }
    }
    else {
        kb_status[scancode] = true;
       kb_last_press = scancode;
       if(active_on_key_press_handler != nullptr) {
           active_on_key_press_handler(scancode);
       }
       if(kb_req) {
           kb_req = false;
           kb_block = false; 
       }
    }
}

unsigned char get_key() {
    kb_req = true;
    kb_block = true;
    while(kb_block) {
        //spin
    }
    return kb_last_press;
}

void initialize() {
    for(int i = 0; i < 128; i++) {
        kb_status[i] = false;
    }
    kb_block = false;
    arch::irq::install_handler(KB_IRQ_CODE, keyboard_handler);
}

/* KBDUS means US Keyboard Layout. This is a scancode table
*  used to layout a standard US keyboard. I have left some
*  comments in to give you an idea of what key is what, even
*  though I set it's array index to 0. You can change that to
*  whatever you want using a macro, if you wish! */
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

char get_key_ascii() {
    return kbdus[get_key()];
}

};