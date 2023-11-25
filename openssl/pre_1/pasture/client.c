
#include <stdio.h>
#include <unistd.h>

#include <openssl/ssl.h>

#define IP  "127.0.0.1"
#define PORT "9999"

#define BUFFER_SIZE 1024

int main(){

	char buffer[BUFFER_SIZE];

	int char_in, char_out;

	BIO *bio;

	SSL_load_error_strings();
	SSL_library_init();

	bio = BIO_new_connect(IP ":" PORT);
	BIO_do_connect(bio);

	while((char_in = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0){
		char_out = 0;
		while(char_in - char_out){
			char_out = BIO_write(bio, buffer + char_out, char_in - char_out);
		}
	}

	return(0);
}
