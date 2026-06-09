.text
.globl timer_interrupt_handler
.globl keyboard_interrupt_handler
.extern c_interrupt_dispatcher   # 声明 C 语言的中断总调度

# 1. 时钟中断入口 (0x20)
timer_interrupt_handler:
    pushl $32                    # 压入中断号 32
    jmp common_interrupt_handler

# 2. 键盘中断入口 (0x21)
keyboard_interrupt_handler:
    pushl $33                    # 压入中断号 33
    jmp common_interrupt_handler

# 👑 3. 统一汇编保护壳
common_interrupt_handler:
    pusha                        # 现场保护：压入 EAX, ECX, EDX...
    
    pushl %esp                   # 🌟 绝妙：把当前的栈指针作为参数压栈传给 C 函数
    call c_interrupt_dispatcher
    addl $4, %esp                # 清除参数栈
    
    # 物理通知 8259A PIC 芯片发送 EOI 信号
    movb $0x20, %al
    outb %al, $0x20
    
    popa                         # 恢复寄存器现场
    addl $4, %esp                # 跳过最开始压入的中断号
    iret                         # 退出中断！k
