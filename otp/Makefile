CC=gcc

CFLAGS= -std=gnu99 -Wall -Wextra -pedantic

all: otp_enc otp_dec

otp_enc: otp_enc.c
	$(CC) $(CFLAGS) -o otp_enc otp_enc.c

otp_dec: otp_dec.c
	$(CC) $(CFLAGS) -o otp_dec otp_dec.c

clean:
	rm otp_enc otp_dec
