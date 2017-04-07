#ifndef DRIVER_KEYBOARD_H
#define DRIVER_KEYBOARD_H

namespace keyboard {

    struct key_info_t {
        unsigned char keycode;
        bool released;
        bool shift_down;
        bool ctrl_down;
    };

    typedef void (*keyevent_handler_t)(key_info_t key);

    extern keyevent_handler_t active_key_handler;

    void initialize();
};
#endif
