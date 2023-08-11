
CC = gcc
CFLAGS = -nostdlib

AS = nasm
AS_FLAGS = -f elf32

hw32: hw32.asm
	$(AS) $(AS_FLAGS) hw32.asm
	$(CC) $(CFLAGS) -o hw32 hw32.o

clean:
	rm hw32 hw32.o
