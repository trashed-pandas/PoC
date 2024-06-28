CC = /usr/bin/gcc
CFLAGS = -std=gnu99 -Wall -Wextra -pedantic -Os
LIBS = -lssl -lpthread

OBJS = common.o

all: client server

server: server.c $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o server server.c

client: client.c $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o client client.c

common: common.c common.h
	$(CC) $(LIBS) $(CFLAGS) -c -o common.o common.c

clean:
	rm client server $(OBJS)
