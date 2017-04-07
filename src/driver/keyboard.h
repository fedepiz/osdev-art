#ifndef DRIVER_KEYBOARD_H
#define DRIVER_KEYBOARD_H

namespace keyboard {

    struct key_info_t {
        unsigned char keycode;
        bool released;
        bool shift_down;
        bool ctrl_down;
    };

    const unsigned char SCANCODE_ESCAPE = 0x01;
    const unsigned char SCANCODE_LEFT_CONTROL = 0x1D;
    const unsigned char SCANCODE_LEFT_SHIFT = 0x2A;
    const unsigned char SCANCODE_RIGHT_SHIFT = 0x36;

    typedef void (*keyevent_handler_t)(key_info_t key);

    extern keyevent_handler_t active_key_handler;

    void initialize();
    bool key_is_special(unsigned char keycode);
    bool key_is_modifier(unsigned char keycode);
};
#endif
