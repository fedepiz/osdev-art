#ifndef DRIVER_KEYBOARD_H
#define DRIVER_KEYBOARD_H

void keyboard_initialize();
//Blocks and waits for a key to be pressed
unsigned char get_key();
char get_key_ascii();
#endif
