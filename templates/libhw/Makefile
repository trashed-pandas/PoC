
CC = clang
AR = llvm-ar
RANLIB = llvm-ranlib

CFLAGS = -std=c17 -Wall -Wextra -pedantic
AR_FLAGS = ruv


all: hello_world 

libhw.o: libhw.c libhw.h
	$(CC) $(CFLAGS) -c libhw.c -o libhw.o

libhw.a: libhw.o
	$(AR) $(AR_FLAGS) libhw.a libhw.o
	$(RANLIB) libhw.a

hello_world: hello_world.c libhw.a
	$(CC) $(CFLAGS) -o hello_world hello_world.c libhw.a

clean:
	rm hello_world *.o *.a
