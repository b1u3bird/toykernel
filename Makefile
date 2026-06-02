CC = gcc
LD = ld
QEMU = qemu-system-i386

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -fno-pie
ASFLAGS = -m32
LDFLAGS = -m elf_i386 -T linker.ld -no-pie

TARGET = mykernel.bin
OBJS = boot.o kernel.o io.o

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET)
	@echo "[SUCCESS] Kernel image $(TARGET) generated"

boot.o: boot.S
	$(CC) $(ASFLAGS) -c boot.S -o boot.o

kernel.o: kernel.c vga.h io.h
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

io.o: io.c io.h vga.h
	$(CC) $(CFLAGS) -c io.c -o io.o

run:all
	$(QEMU) -kernel $(TARGET)

clean:
	rm -f *.o $(TARGET)
	@echo "[CLEAN]"
