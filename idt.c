#include "idt.h"
#include "io.h"
#include "keyboard.h"
#include "vga.h"
struct idt_entry idt[256];
struct idt_ptr idtp;
struct pt_regs {
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx,
        eax; // 8 general-purpose registers
    unsigned int interrupt_number;
};
static inline void idt_set_gate(unsigned char num, unsigned int base,
                                unsigned short sel, unsigned char flags) {
    idt[num].base_low = (unsigned short)(base & 0xFFFF);
    idt[num].base_high = (unsigned short)((base >> 16) & 0xFFFF);
    idt[num].selector = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}
static void pic_remap(void) {
    // 初始化
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20); // 主PIC的中断向量起始地址
    outb(0xA1, 0x28); // 从PIC的中断向量起始地址

    outb(0x21, 0x04); // 主PIC告诉从PIC它连接在IRQ2上
    outb(0xA1, 0x02); // 从PIC告诉主PIC它连接在IRQ2上

    outb(0x21, 0x01); // 设置主PIC为8086模式
    outb(0xA1, 0x01); // 设置从PIC为8086模式

    outb(0x21, 0xFC);
    outb(0xA1, 0xFF);
}

void idt_init() {
    pic_remap();
    extern void timer_interrupt_handler();
    extern void keyboard_interrupt_handler();

    idt_set_gate(32, (unsigned int)timer_interrupt_handler, 0x08, 0x8E);
    idt_set_gate(33, (unsigned int)keyboard_interrupt_handler, 0x08, 0x8E);

    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (unsigned int)&idt;

    __asm__ volatile("lidt (%0)" : : "m"(idtp));
}

void c_interrupt_dispatcher(struct pt_regs *regs) {
    if (regs->interrupt_number == 32) {
        static int ticks = 0;
        ticks++;
        if (ticks % 100 == 0) {
            printk("Timer Interrupt: %d ticks\n", ticks);
        }
    } else if (regs->interrupt_number == 33) {
        extern void handle_raw_scancode(unsigned char scancode);
	unsigned char scancode = inb(0x60);
	handle_raw_scancode(scancode);
    }
}
