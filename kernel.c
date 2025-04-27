#include "cmds.h"

volatile char *video = (volatile char*)0xB8000;
int cursor = 0;

#define INPUT_BUFFER_SIZE 128
char input_buffer[INPUT_BUFFER_SIZE];
int input_pos = 0;

void print_char_color(char c, unsigned char color) {
    if (c == '\n') {
        cursor += (160 - (cursor % 160));
    } else {
        video[cursor++] = c;
        video[cursor++] = color;
    }
}

void print_char(char c) {
    print_char_color(c, 0x07);
}

void print(const char *str) {
    while (*str) {
        print_char(*str++);
    }
}

void clear(void) {
    for (int i = 0; i < 80 * 25 * 2; i++) {
        video[i] = 0;
    }
    cursor = 0;
}

unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

unsigned char keyboard_ready() {
    return inb(0x64) & 1;
}

const char keymap[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

void print_prompt() {
    print("Onexis:");
    print(get_current_path());
    print("> ");
}

void keyboard_handler() {
    unsigned char scancode = inb(0x60);

    if (scancode & 0x80) {

    } else {
        if (scancode < sizeof(keymap)) {
            char c = keymap[scancode];
            if (c == '\b') {
                if (input_pos > 0) {
                    input_pos--;
                    cursor -= 2;
                    video[cursor] = ' ';
                    video[cursor + 1] = 0x07;
                }
            } else if (c) {
                print_char(c);
                if (input_pos < INPUT_BUFFER_SIZE - 1) {
                    input_buffer[input_pos++] = c;
                }
            }
            if (c == '\n') {
                input_buffer[input_pos - 1] = '\0';
                if (input_pos > 1) {
                    execute_command(input_buffer);
                }
                input_pos = 0;
                print_prompt();
            }
        }
    }
}

void kernel_main(void) {
    clear();
    init_fs();
    print("Welcome to Onexis!\n");
    print_prompt();

    while (1) {
        if (keyboard_ready()) {
            keyboard_handler();
        }
    }
}
