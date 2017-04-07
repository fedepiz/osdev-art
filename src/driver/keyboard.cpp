#include <driver/keyboard.h>
#include <kernel/arch.h>
#include <util/text.h>
#include <kstdio.h>
namespace keyboard {

using arch::inb;

    keyevent_handler_t active_key_handler;


    void keyboard_handler(struct arch::regs *r) {
        (void)r;
        unsigned char scancode;

        /* Read from the keyboard's data buffer */
        scancode = inb(0x60);
        key_info_t key_info;

        /* If the top bit of the byte we read from the keyboard is
        *  set, that means that a key has just been released */
        if (scancode & 0x80) {
            /* You can use this one to see if the user released the
            *  shift, alt, or control keys... */
            key_info.released = true;
            
        }
        else {
            key_info.released = false;
        }

        key_info.keycode = scancode & 0x7F;
        if(active_key_handler != nullptr) {
                active_key_handler(key_info);
        }
    }



    void initialize() {
        arch::irq::install_handler(KB_IRQ_CODE, keyboard_handler);
    }
};