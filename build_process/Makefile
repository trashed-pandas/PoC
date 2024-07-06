
CC = /usr/bin/clang
CFLAGS = -pedantic -ansi -Wall -Wextra -masm=intel


all: hw

hw.i: hw.c
	$(CC) -E -o hw.i hw.c

hw.s: hw.i
	$(CC) $(CFLAGS) -o hw.s -S hw.i

hw.o: hw.s
	$(CC) -o hw.o -c hw.s

hw: hw.o
	$(CC) -o hw	hw.o

clean:
	/bin/rm hw.i hw.s hw.o hw
