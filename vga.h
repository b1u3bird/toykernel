// vga.h - VGA 文本模式硬件常量定义
#ifndef VGA_H
#define VGA_H

// 显存文本模式的物理映射基地址
#define VIDEO_ADDRESS 0xB8000

// 【极客扩展】你后续一定会用到的 VGA 颜色属性宏，也可以一并收拢在这
#define VGA_COLOR_BLACK         0x00
#define VGA_COLOR_WHITE         0x07
#define VGA_COLOR_LIGHT_GREEN   0x0A

// 屏幕尺寸定义
#define VGA_SCREEN_WIDTH        80
#define VGA_SCREEN_HEIGHT       25

#endif
