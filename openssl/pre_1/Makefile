CC = /usr/bin/gcc
CFLAGS = -std=gnu99 -Wall -Wextra -pedantic -Os
LIBS = -lssl

all: ssl_io

ssl_io: ssl_io.c
	$(CC) $(CFLAGS) $(LIBS) -o ssl_io ssl_io.c

clean:
	rm ssl_io
