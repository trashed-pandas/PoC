CC = /usr/bin/clang
CFLAGS = -std=c18 -Wall -Wextra -pedantic -O0 -ldl

encrypt: encrypt.c
	$(CC) $(CFLAGS) -o encrypt encrypt.c /usr/local/lib64/libssl.a /usr/local/lib64/libcrypto.a

clean:
	rm encrypt
