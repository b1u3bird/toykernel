#include "idt.h"
#include "keyboard.h"
#include "string.h"
#include "vga.h"
char cmd_buf[128];
int buf_siz = 128;
int buf_pos = 0;
void kernel_main(unsigned long magic, unsigned long addr) {
    // 1. 初始化你的格式化输出基础设施
    clear_screen();
    printk("ToyOS Kernel Booting...\n");

    // 2. 加载中断大厦
    idt_init();
    printk("IDT and PIC configured.\n");

    // 3. 👑 临门一脚：执行 sti 指令，正式宣告 CPU 开启中断接听！
    __asm__ volatile("sti");
    printk("Interrupts enabled globally. Try press any key now!\n");
    printk("ToyOS_Shell>");
    // 4. 内核死循环，静静等待硬件中断去打破安宁
    while (1) {
        // CPU 可以在这里闲逛，一旦敲键盘，就会瞬间被硬核切入执行 do_keyboard
        char c = keyboard_getchar();
        if (c == 0) {
            __asm__ volatile("hlt");
            continue;
        }
        putchar(c);
        if (c == '\n') {
            memset(cmd_buf, 0, buf_pos);
            buf_pos = 0;
            printk("ToyOS_Shell>");
        } else if (c == '\b') {
            if (buf_pos > 0)
                cmd_buf[--buf_pos] = '\0';
        }
    }
}
