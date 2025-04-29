#include "cmds.h"
#include "keyboard.h"  // Додаємо заголовок для роботи з клавіатурою

volatile char *video = (volatile char*)0xB8000;
int cursor = 0;
int cursor_visible = 1;
int last_cursor_update = 0;
#define CURSOR_BLINK_INTERVAL 500000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

int get_time() {
    static int time = 0;
    return time++;  // Лічильник часу для тесту
}

void update_cursor() {
    int current_time = get_time();
    if (current_time - last_cursor_update >= CURSOR_BLINK_INTERVAL) {
        cursor_visible = !cursor_visible;
        last_cursor_update = current_time;
    }
}

void print_char_color(char c, unsigned char color) {
    if (c == '\n') {
        cursor += (160 - (cursor % 160));
    } else {
        video[cursor++] = c;
        video[cursor++] = color;
    }

    if (cursor >= SCREEN_WIDTH * SCREEN_HEIGHT * 2) {
        cursor = 0;
    }
}

void print_prompt() {
    print("Onexis: ");
    print(get_current_path());
    print("> ");
    cursor_visible = 1;
}

void clear() {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT * 2; i++) {
        video[i] = 0;
    }
    cursor = 0;
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

        update_cursor(); 
    }
}
