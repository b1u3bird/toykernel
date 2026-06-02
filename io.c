#include "io.h"
#include "keyboard.h"
#include "vga.h"
unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ volatile("inb %1,%0" : "=a"(result) : "Nd"(port));
    return result;
}
void outb(unsigned short port, unsigned char data) {
    __asm__ volatile("outb %0,%1" ::"a"(data), "Nd"(port));
}

int get_cursor_position() {
    int position = 0;
    outb(VGA_CRT_INDEX_PORT, VGA_CURSOR_HIGH_IDX);
    position |= ((int)inb(VGA_CRT_DATA_PORT)) << 8;
    outb(VGA_CRT_INDEX_PORT, VGA_CURSOR_LOW_IDX);
    position |= ((int)inb(VGA_CRT_DATA_PORT));
    return position;
}

void update_cursor_position(int pos) {
    outb(VGA_CRT_INDEX_PORT, VGA_CURSOR_HIGH_IDX);
    outb(VGA_CRT_DATA_PORT, (unsigned char)((pos >> 8) & 0xFF));
    outb(VGA_CRT_INDEX_PORT, VGA_CURSOR_LOW_IDX);
    outb(VGA_CRT_DATA_PORT, (unsigned char)((pos) & 0xFF));
}
void update_cursor_position_xy(int x, int y) {
    int position = y * VGA_SCREEN_WIDTH + x;
    update_cursor_position(position);
}
void clear_screen() {
    volatile char *video_memory = (volatile char *)VIDEO_ADDRESS;
    int total_bytes = VGA_SCREEN_HEIGHT * VGA_SCREEN_WIDTH * 2;
    for (int i = 0; i < total_bytes; i += 2) {
        video_memory[i] = ' ';
    }
}

char keyboard_getchar() {
    while (!(inb(KEYBOARD_STATUS_PORT) & 1))
        ;
    unsigned char scancode = inb(KEYBOARD_DATA_PORT);
    if (scancode & 0x80)
        return 0;
    return scan_code_table[scancode];
}
