#include <driver/keyboard.h>
#include <kernel/arch.h>
#include <util/text.h>
#include <kstdio.h>
namespace keyboard {

using arch::inb;
using util::logf; 
    keyevent_handler_t active_key_handler;

    bool shift_down = false;
    bool ctrl_down = false;

    void keyboard_handler(struct arch::regs *r) {
        (void)r;
        unsigned char scancode;

        /* Read from the keyboard's data buffer */
        scancode = inb(0x60);
        unsigned char keycode = scancode & 0x7F;
        key_info_t key_info;

        /* If the top bit of the byte we read from the keyboard is
        *  set, that means that a key has just been released */
        if (scancode & 0x80) {
            /* You can use this one to see if the user released the
            *  shift, alt, or control keys... */
            key_info.released = true;

            if(keycode == SCANCODE_LEFT_CONTROL) {
                ctrl_down = false;
            }
            if(
                keycode == SCANCODE_LEFT_SHIFT || 
                keycode == SCANCODE_RIGHT_SHIFT) {
                    shift_down = false;
            }
            
            //check for control key - if so not pressed, but just set the modifier
        }
        else {
            key_info.released = false;

            if(keycode == SCANCODE_LEFT_CONTROL) {
                ctrl_down = true;
            }
            if(
                keycode == SCANCODE_LEFT_SHIFT || 
                keycode == SCANCODE_RIGHT_SHIFT) {
                    shift_down = true;
            }
        }

        key_info.keycode = keycode;
        key_info.shift_down = shift_down;
        key_info.ctrl_down = ctrl_down;
        //logf("Key is %x, released is %b, Shift is is %b\n", 
        //   key_info.keycode, key_info.released, key_info.shift_down);
        if(active_key_handler != nullptr) {
                active_key_handler(key_info);
        }
    }



    void initialize() {
        shift_down = false;
        ctrl_down = false;
        arch::irq::install_handler(KB_IRQ_CODE, keyboard_handler);
    }
    bool key_is_special(unsigned char keycode) {
        return keycode == SCANCODE_ESCAPE ||
               key_is_modifier(keycode);
    }

    bool key_is_modifier(unsigned char keycode) {
        return keycode == SCANCODE_LEFT_CONTROL ||
               keycode == SCANCODE_LEFT_SHIFT ||
               keycode == SCANCODE_RIGHT_SHIFT;
    }
};