
# ➤ cat /etc/ld.so.conf.d/local.conf
#   /usr/local/lib
#   /usr/local/lib64
# ➤ ldconfig 

CC = /usr/bin/clang
CFLAGS = -std=c18 -Wall -Wextra -pedantic -O0 -L/usr/local/lib64 -I/usr/local/include
#LIBS = /usr/local/lib64/libssl.a /usr/local/lib64/libcrypto.a
LIBS = -lssl -lcrypto

encrypt: encrypt.c
	$(CC) $(CFLAGS) -o encrypt encrypt.c $(LIBS)

clean:
	rm encrypt
