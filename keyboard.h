// keyboard.h - 键盘驱动头文件
#ifndef KEYBOARD_H
#define KEYBOARD_H

// 键盘数据端口
#define KEYBOARD_DATA_PORT 0x60
// 键盘状态端口
#define KEYBOARD_STATUS_PORT 0x64

// 扫描码映射表（标准美式键盘映射）
// 数组下标对应硬件扫描码，值对应 ASCII 码
static const char scan_code_table[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0,
  '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,   '*',   0,
   ' ',   0
};

// 驱动函数声明
char keyboard_get_char();

#endif
