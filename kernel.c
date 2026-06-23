#include "idt.h"
#include "io.h"
#include "keyboard.h"
#include "string.h"
#include "vga.h"
char cmd_buf[128];
int buf_siz = 128;
int buf_pos = 0;
char *cmd_list[] = {
    "help",  "clear", "reboot", "shutdown", "echo", "ls",   "cat",  "mkdir",
    "rmdir", "touch", "rm",     "cp",       "mv",   "find", "grep", "wc",
    "date",  "time",  "whoami", "uname",    "ps",   "kill",
};
void reboot() {
    // 通过向 0x64 端口写入 0xFE 来触发软重启
    outb(0x64, 0xFE);
}
void shutdown() { outw(0x604, 0x2000); }
void handle_command(char *cmd) {
    if (strcmp(cmd, "help") == 0) {
        printk("Available commands:\n");
        for (int i = 0; i < sizeof(cmd_list) / sizeof(cmd_list[0]); i++) {
            printk("  %s\n", cmd_list[i]);
        }
    } else if (strcmp(cmd, "clear") == 0) {
        clear_screen();
    } else if (strcmp(cmd, "reboot") == 0) {
        reboot();
    } else if (strcmp(cmd, "shutdown") == 0) {
        shutdown();
    } else if (strncmp(cmd, "echo ", 5) == 0) {
        printk("%s\n", cmd + 5);
    } else {
        printk("Unknown command: %s\n", cmd);
    }
}
void cmd_putchar(char c) {
    if (c == '\n') {
        putchar(c);
        if (buf_pos > 0) {
            handle_command(cmd_buf);
            memset(cmd_buf, 0, buf_pos);
            buf_pos = 0;
        }
        printk("ToyOS_Shell>");
    } else if (c == '\b') {
        if (buf_pos > 0) {
            putchar(c);
            cmd_buf[--buf_pos] = '\0';
        }
    } else {
        if (buf_pos == buf_siz - 1) {
            printk("\nCommand buffer full! Please press Enter to execute the "
                   "command.\n");
            return;
        }
        putchar(c);
        cmd_buf[buf_pos++] = c;
    }
}
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
    memset(cmd_buf, 0, buf_siz);
    printk("ToyOS_Shell>");
    // 4. 内核死循环，静静等待硬件中断去打破安宁
    while (1) {
        // CPU 可以在这里闲逛，一旦敲键盘，就会瞬间被硬核切入执行
        char c = keyboard_getchar();
        if (c == 0) {
            __asm__ volatile("hlt");
            continue;
        }
        cmd_putchar(c);
    }
}
