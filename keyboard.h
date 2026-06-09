// keyboard.h - 键盘驱动头文件
#ifndef KEYBOARD_H
#define KEYBOARD_H

// 键盘数据端口
#define KEYBOARD_DATA_PORT 0x60
// 键盘状态端口
#define KEYBOARD_STATUS_PORT 0x64
void handle_raw_scancode(unsigned char scancode);

#endif
