/*
 *	otp_enc - Baigu's (pseudo) one time pad encrypter.
 *		Intended for obfuscation. Do not rely on this for actual crypto.
 *
 *	Takes plaintext on stdin.
 *	Prints ciphertext on stdout.
 *	Prints key to stderr.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(){
	int i, retval;
	char text[sizeof(int)];

	union i2c {
		int number;
		unsigned char character[sizeof(int)];
	} key;


	memset(text, 0, sizeof(text));
	srandom((unsigned int) time(NULL));

	while((retval = read(0, text, sizeof(text)))){

		key.number = random();

		for(i = 0; i < retval; i++){
			if(!text[i]){
				break;
			}

			fprintf(stdout, "\\x%02x", text[i] ^ key.character[i]);
			fprintf(stderr, "\\x%02x", key.character[i]);
		}

		memset(text, 0, sizeof(text));
	}

	return(0);
}
