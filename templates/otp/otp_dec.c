/*
 *	otp_enc - Baigu's (pseudo) one time pad decrypter.
 *		Intended for obfuscation. Do not rely on this for actual crypto.
 *
 *	Takes ciphertext on stdin.
 *	Reads key from KEY_FILE as described in argv.
 *	Prints plaintext to stdout.
 */

#define _GNU_SOURCE

#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char **argv){
	int retval, key_retval, cipher_retval, key_fd;
	char key[5];
	char cipher[5];

	if(argc != 2){
		fprintf(stderr, "usage: %s KEY_FILE\n", program_invocation_name);
		exit(-1);
	}

	if((retval = open(argv[1], O_RDONLY)) == -1){
		error(-1, errno, "open()");
	}
	key_fd = retval;

	memset(key, 0, sizeof(key));
	memset(cipher, 0, sizeof(cipher));

	while(1){
		key_retval = read(key_fd, key, 4);
		cipher_retval = read(0, cipher, 4);
	
		if(key_retval != cipher_retval){
			fprintf(stderr, "%s: ciphertext length differs from key length. Quitting.\n", program_invocation_name);
			exit(-1);
		}
		
		if(!key_retval){
			break;
		}

		printf("%c", (char) (strtol(key+2, NULL, 16) ^ strtol(cipher+2, NULL, 16)));

		memset(key, 0, sizeof(key));
		memset(cipher, 0, sizeof(cipher));
	}

	return(0);
}
