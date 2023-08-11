
#include <stdio.h>
#include <unistd.h>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

#define IP	"127.0.0.1"
#define PORT "9999"

#define BUFFER_SIZE	1024

#define PLAINTEXT 0
#define ADH	1
#define EDH	2


struct io_helper {
	int listener;
	int encryption;

	BIO *connect;

	int (*remote_read)(struct io_helper *io, void *buf, size_t count);
	int (*remote_write)(struct io_helper *io, void *buf, size_t count);

};



int remote_read_plaintext(struct io_helper *io, void *buf, size_t count);



int main(int argc, char **argv){


	char buffer[BUFFER_SIZE];

	int char_in, char_out;

	BIO *accept;

	SSL_library_init();
	SSL_load_error_strings();

	struct io_helper io;

	int opt;
	int opt_counter = 0;

	io.encryption = 0;

	while((opt = getopt(argc, argv, "lpae")) != -1){
		switch(opt){

			case 'l':
				io.listener = 1;
				break;

			case 'p':
				io.encryption = PLAINTEXT;
				opt_counter++;
				break;

			case 'a':
				io.encryption = ADH;
				opt_counter++;
				break;

			case 'e':
				io.encryption = EDH;
				opt_counter++;
				break;

			default:
				exit(-1);
		}
	}

	if(opt_counter > 1){
		fprintf(stderr, "Too many options!\n");
		exit(-1);
	}

	if(io.encryption){
	}else{
		io.remote_read = &remote_read_plaintext;
	}

	if((accept = BIO_new_accept(IP ":" PORT)) == NULL){
		ERR_print_errors_fp(stderr);
		exit(-1);
	}

	if(BIO_set_bind_mode(accept, BIO_BIND_REUSEADDR) <= 0){
		ERR_print_errors_fp(stderr);
		exit(-1);
	}

	if(BIO_do_accept(accept) <= 0){
		ERR_print_errors_fp(stderr);
		exit(-1);
	}

	if(BIO_do_accept(accept) <= 0){
		ERR_print_errors_fp(stderr);
		exit(-1);
	}
	if((io.connect = BIO_pop(accept)) == NULL){
		ERR_print_errors_fp(stderr);
		exit(-1);
	}

	BIO_free(accept);

	while((char_in = io.remote_read(&io, buffer, BUFFER_SIZE)) > 0){
		char_out = 0;
		while(char_in - char_out){
			char_out = write(STDOUT_FILENO, buffer + char_out, char_in - char_out);
			if(char_out < 0){
				ERR_print_errors_fp(stderr);
				exit(-1);
			}
		}
	}
	if(char_in < 0){
		ERR_print_errors_fp(stderr);
		exit(-1);
	}

	return(0);
}

int remote_read_plaintext(struct io_helper *io, void *buf, size_t count){

	return(BIO_read(io->connect, buf, count));

}

