#ifndef IO_H
#define IO_H
unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);
int get_cursor_position();
void update_cursor_position(int pos);
void update_cursor_position_xy(int x, int y);
void clear_screen();
char keyboard_getchar();
#endif
