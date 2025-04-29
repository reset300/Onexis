#include "cmds.h"
#include <time.h>
#include <string.h>  // Підключаємо стандартну бібліотеку для роботи з рядками

#define MAX_FILES 128
#define NAME_SIZE 32

typedef enum {
    TYPE_FILE,
    TYPE_DIR
} FileType;

typedef struct {
    char name[NAME_SIZE];
    FileType type;
    int parent;
} FileEntry;

FileEntry fs[MAX_FILES];
int fs_size = 0;
int current_dir = 0;
char path[NAME_SIZE * 4];

void init_fs() {
    fs[0].type = TYPE_DIR;
    fs[0].parent = -1;
    strcpy(fs[0].name, "/");
    fs_size = 1;
    strcpy(fs[fs_size].name, "home");
    fs[fs_size].type = TYPE_DIR;
    fs[fs_size].parent = 0;
    fs_size++;
    strcpy(fs[fs_size].name, "bin");
    fs[fs_size].type = TYPE_DIR;
    fs[fs_size].parent = 0;
    fs_size++;
    strcpy(fs[fs_size].name, "mnt");
    fs[fs_size].type = TYPE_DIR;
    fs[fs_size].parent = 0;
    fs_size++;
}

const char* get_current_path() {
    int temp = current_dir;
    path[0] = '\0';
    char temp_path[NAME_SIZE * 4];
    temp_path[0] = '\0';
    while (temp != -1) {
        char buffer[NAME_SIZE + 2];
        buffer[0] = '/';
        strcpy(buffer + 1, fs[temp].name);
        strcpy(temp_path + strlen(buffer), temp_path);
        strcpy(temp_path, buffer);
        temp = fs[temp].parent;
    }
    strcpy(path, temp_path);
    return path;
}

extern void print_char_color(char c, unsigned char color);

void execute_command(const char *input) {
    if (!input || !input[0]) return;

    if (strncmp(input, "echo ", 5) == 0) {
        print(input + 5);
        print("\n");
    } else if (strncmp(input, "mkdir ", 6) == 0) {
        if (fs_size < MAX_FILES) {
            strcpy(fs[fs_size].name, input + 6);
            fs[fs_size].type = TYPE_DIR;
            fs[fs_size].parent = current_dir;
            fs_size++;
            print("[mkdir] Directory created.\n");
        } else {
            print("[mkdir] No space left.\n");
        }
    } else if (strncmp(input, "mkfile ", 7) == 0) {
        if (fs_size < MAX_FILES) {
            strcpy(fs[fs_size].name, input + 7);
            fs[fs_size].type = TYPE_FILE;
            fs[fs_size].parent = current_dir;
            fs_size++;
            print("[mkfile] File created.\n");
        } else {
            print("[mkfile] No space left.\n");
        }
    } else if (strcmp(input, "ls") == 0) {
        for (int i = 0; i < fs_size; i++) {
            if (fs[i].parent == current_dir && fs[i].name[0]) {
                if (fs[i].type == TYPE_DIR) {
                    for (int j = 0; fs[i].name[j]; j++) {
                        print_char_color(fs[i].name[j], 0x02);
                    }
                } else {
                    for (int j = 0; fs[i].name[j]; j++) {
                        print_char_color(fs[i].name[j], 0x01);
                    }
                }
                print("\n");
            }
        }
    } else if (strncmp(input, "rm ", 3) == 0) {
        const char *name = input + 3;
        for (int i = 0; i < fs_size; i++) {
            if (fs[i].parent == current_dir && strcmp(fs[i].name, name) == 0) {
                fs[i].name[0] = 0;
                print("[rm] Deleted.\n");
                return;
            }
        }
        print("[rm] Not found.\n");
    } else if (strncmp(input, "cd ", 3) == 0) {
        const char *name = input + 3;
        if (strcmp(name, "..") == 0) {
            if (fs[current_dir].parent != -1) {
                current_dir = fs[current_dir].parent;
            }
        } else if (strcmp(name, "/") == 0) {
            current_dir = 0;
        } else {
            int found = 0;
            for (int i = 0; i < fs_size; i++) {
                if (fs[i].parent == current_dir && strcmp(fs[i].name, name) == 0 && fs[i].type == TYPE_DIR) {
                    current_dir = i;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                print("[cd] Directory not found.\n");
            }
        }
        clear();
        print_prompt();
    } else if (strcmp(input, "date") == 0) {
        time_t t;
        struct tm *tm_info;
        char buffer[26];
        time(&t);
        tm_info = localtime(&t);
        strftime(buffer, 26, "%Y-%m-%d", tm_info);
        print(buffer);
        print("\n");
    } else if (strcmp(input, "time") == 0) {
        time_t t;
        struct tm *tm_info;
        char buffer[26];
        time(&t);
        tm_info = localtime(&t);
        strftime(buffer, 26, "%H:%M:%S", tm_info);
        print(buffer);
        print("\n");
    } else {
        print("[error] Unknown command.\n");
    }
}
