#ifndef IDT_H
#define IDT_H
#pragma pack(push, 1)
struct idt_entry {
    unsigned short base_low;
    unsigned short selector;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_high;
};
struct idt_ptr {
    unsigned short limit;
    unsigned int base;
};
#pragma pack(pop)
static inline void idt_set_gate(unsigned char num, unsigned int base,
                                unsigned short sel, unsigned char flags);
void idt_init(void);
#endif
