#ifndef CMDS_H
#define CMDS_H

void execute_command(const char *input);
void print(const char *str);
void clear(void);
void init_fs(void);
const char* get_current_path(void);
void print_prompt(void);  // Додали оголошення print_prompt

#endif
