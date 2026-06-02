.set MAGIC, 0x1BADB002
.set FLAGS, 0x00000003
.set CHECKSUM,-(MAGIC+FLAGS)


.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text
.global _start
.type _start,@function


_start:
    movl $stack_top,%esp
    pushl %ebx   #MBI结构体指针,含有内存大小等值
    pushl %eax   #multiboot 魔法数

    .extern kernel_main
    call kernel_main


_halt:
    cli 
    hlt 
    jmp _halt
