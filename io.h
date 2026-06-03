#ifndef IO_H
#define IO_H
unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);
void get_cursor_position();
void update_cursor_position();
void clear_screen();
char keyboard_getchar();
void putchar(char c);
void printk(const char *fmt, ...);
#endif
