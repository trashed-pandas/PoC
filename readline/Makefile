CC = /usr/bin/gcc
#CFLAGS = -std=c99 -Wall -Wextra -pedantic -Os -static
CFLAGS = -std=c99 -Wall -Wextra -pedantic -Os

OBJS = linenoise.o

all: test

test: test.c $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o test test.c

linenoise.o:
	$(CC) $(CFLAGS) -I./linenoise $(LIBS) -c -o linenoise.o linenoise/linenoise.c

#test: test.c
#	$(CC) $(CFLAGS) $(LIBS) -o test test.c -lreadline -ltermcap

clean:
	rm test
