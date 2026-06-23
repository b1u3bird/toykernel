#include "keyboard.h"
#include "vga.h"
#define BUFSIZ 1024
char buffer[BUFSIZ];
int head = 0;
int tail = 0;
static char key_map[128] = {
    0,   27,  '1',  '2',  '3',  '4', '5', '6',  '7', '8', '9', '0',
    '-', '=', '\b', '\t', 'q',  'w', 'e', 'r',  't', 'y', 'u', 'i',
    'o', 'p', '[',  ']',  '\n', 0,   'a', 's',  'd', 'f', 'g', 'h',
    'j', 'k', 'l',  ';',  '\'', '`', 0,   '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm',  ',',  '.',  '/', 0,   '*',  0,   ' '};
void handle_raw_scancode(unsigned char scancode) {
    if (scancode & 0x80)
        return;
    if (scancode > 127)
        return;
    char c = key_map[scancode];
    buffer[head] = c;
    head = (head + 1) % BUFSIZ;
}
char keyboard_getchar() {
    if (head == tail)
        return 0;
    char c = buffer[tail];
    tail = (tail + 1) % BUFSIZ;
    return c;
}
