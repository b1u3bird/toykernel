#include "vga.h"
#include "io.h"
#include <stdarg.h>
int cursor_x = 0;
int cursor_y = 0;
void get_cursor_position() {
    int position = 0;
    outb(VGA_CRT_INDEX_PORT, VGA_CURSOR_HIGH_IDX);
    position |= ((int)inb(VGA_CRT_DATA_PORT)) << 8;
    outb(VGA_CRT_INDEX_PORT, VGA_CURSOR_LOW_IDX);
    position |= ((int)inb(VGA_CRT_DATA_PORT));
    cursor_x = position % VGA_SCREEN_WIDTH;
    cursor_y = position / VGA_SCREEN_WIDTH;
}

void update_cursor_position() {
    int pos = cursor_x + cursor_y * VGA_SCREEN_WIDTH;
    outb(VGA_CRT_INDEX_PORT, VGA_CURSOR_HIGH_IDX);
    outb(VGA_CRT_DATA_PORT, (unsigned char)((pos >> 8) & 0xFF));
    outb(VGA_CRT_INDEX_PORT, VGA_CURSOR_LOW_IDX);
    outb(VGA_CRT_DATA_PORT, (unsigned char)((pos) & 0xFF));
}

void clear_screen() {
    volatile char *video_memory = (volatile char *)VIDEO_ADDRESS;
    int total_bytes = VGA_SCREEN_HEIGHT * VGA_SCREEN_WIDTH * 2;
    for (int i = 0; i < total_bytes; i += 2) {
        video_memory[i] = ' ';
    }
}

void itoa(int x, char *str, int base) {
    char *ptr;
    char *low;
    char *rc;
    rc = ptr = str;
    if (x < 0) {
        *ptr++ = '-';
        x = -x;
    }
    low = ptr;
    do {
        *ptr++ = "0123456789abcdef"[x % base];
        x /= base;
    } while (x);
    *ptr++ = '\0';
    ptr--;
    while (low < ptr) {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
}

int vsprintf(char *buf, const char *fmt, va_list args) {
    char *p = buf;
    while (*fmt) {
        if (*fmt != '%') {
            *p++ = *fmt++;
            continue;
        }
        fmt++;
        switch (*fmt) {
        case 'c': {
            *p++ = (char)va_arg(args, int);
            break;
        }
        case 's': {
            char *s = va_arg(args, char *);
            while (*s) {
                *p++ = *s++;
            }
            break;
        }
        case 'd': {
            char num_str[32];
            itoa(va_arg(args, int), num_str, 10);
            char *s = num_str;
            while (*s) {
                *p++ = *s++;
            }
            break;
        }
        case 'x': {
            char num_str[32];
            itoa(va_arg(args, int), num_str, 16);
            char *s = num_str;
            while (*s) {
                *p++ = *s++;
            }
            break;
        }
        default:
            *p++ = *fmt;
            break;
        }
        fmt++;
    }
    *p = '\0';
    return p - buf;
}
void putchar(char c) {
    volatile char *video_memory = (volatile char *)VIDEO_ADDRESS;
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
	    int pos = cursor_x + cursor_y * VGA_SCREEN_WIDTH;
	    int offset = pos * 2;
	    video_memory[offset] = ' ';
	    video_memory[offset + 1] = VGA_COLOR_LIGHT_GREEN;
        }
    } else if (c == '\t') {
        int nexttab_x = (cursor_x / 8 + 1) * 8;
        while (cursor_x < nexttab_x && cursor_x < VGA_SCREEN_WIDTH) {
            int pos = cursor_x + cursor_y * VGA_SCREEN_WIDTH;
            int offset = pos * 2;
            video_memory[offset] = ' ';
            video_memory[offset + 1] = VGA_COLOR_WHITE;
            cursor_x++;
        }
    } else {
        int pos = cursor_x + cursor_y * VGA_SCREEN_WIDTH;
        int offset = pos * 2;
        video_memory[offset] = c;
        video_memory[offset + 1] = VGA_COLOR_LIGHT_GREEN;
        cursor_x++;
        if (cursor_x >= VGA_SCREEN_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }
    update_cursor_position();
}
void printk(const char *fmt, ...) {
    char buf[1024];
    va_list args;

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    for (int i = 0; buf[i] != '\0'; i++) {
        putchar(buf[i]);
    }
}
