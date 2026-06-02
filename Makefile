CC = gcc
lD = ld
QEMU = qemu-system-i386

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -fno-pie
ASFLAGS = -m32
LDFLAGS = -m elf_i386 -T linker.ld -no-pie

TARGET = mykernel.bin
OBJS = boot.o kernel.o

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET)
	@echo "[SUCCESS] Kernel image $(TARGET) generated"

boot.o: boot.S
	$(CC) $(ASFLAGS) -c boot.S -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

run:all
	$(QEMU) -kernel $(TARGET)

clean:
	rm -f *.o $(TARGET)
	@echo "[CLEAN]"
