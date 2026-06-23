CC = gcc
LD = ld
QEMU = qemu-system-i386

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -fno-pie 
ASFLAGS = -m32
LDFLAGS = -m elf_i386 -T linker.ld -no-pie

TARGET = mykernel.bin
OBJS = boot.o interrupt.o kernel.o  keyboard.o idt.o vga.o string.o

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET)
	@echo "[SUCCESS] Kernel image $(TARGET) generated"

boot.o: boot.S
	$(CC) $(ASFLAGS) -c boot.S -o boot.o

interrupt.o:interrupt.S
	$(CC) $(ASFLAGS) -c interrupt.S -o interrupt.o

kernel.o: kernel.c vga.h io.h keyboard.h string.h
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

idt.o: idt.c idt.h io.h keyboard.h
	$(CC) $(CFLAGS) -c idt.c -o idt.o

vga.o: vga.c vga.h io.h
	$(CC) $(CFLAGS) -c vga.c -o vga.o

keyboard.o: keyboard.c vga.h keyboard.h io.h
	$(CC) $(CFLAGS) -c keyboard.c -o keyboard.o

string.o: string.c string.h
	$(CC) $(CFLAGS) -c string.c -o string.o

run:all
	$(QEMU) -kernel $(TARGET)

clean:
	rm -f *.o $(TARGET)
	@echo "[CLEAN]"
