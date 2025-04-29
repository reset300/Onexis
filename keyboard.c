#include "keyboard.h"
#include <stdint.h>

#define KEYBOARD_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

unsigned char keyboard_ready() {
    return inb(KEYBOARD_STATUS_PORT) & 1;
}

void keyboard_handler() {
    unsigned char scancode = inb(KEYBOARD_PORT);

    // Тут ми можемо додати обробку скан-коду, для прикладу просто вивести його
    print_char(scancode);  // Виводимо скан-код натиснутої клавіші
}
