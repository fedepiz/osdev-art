#ifndef DRIVER_KEYBOARD_H
#define DRIVER_KEYBOARD_H

namespace keyboard {
void initialize();
//Blocks and waits for a key to be pressed
unsigned char get_key();
char get_key_ascii();

typedef void (*keyevent_handler_t)(unsigned char keycode);

extern keyevent_handler_t active_on_key_press_handler;
extern keyevent_handler_t active_on_key_release_handler;
};
#endif
