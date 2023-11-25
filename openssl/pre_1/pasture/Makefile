CC = /usr/bin/gcc
CFLAGS = -std=gnu99 -Wall -Wextra -pedantic -Os
LIBS = -lssl

all: client server

server: server.c
	$(CC) $(CFLAGS) $(LIBS) -o server server.c

client: client.c
	$(CC) $(CFLAGS) $(LIBS) -o client client.c

clean:
	rm client server
