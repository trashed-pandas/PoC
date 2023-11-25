
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>

#define IP	"127.0.0.1"
#define PORT "9999"

#define BUFFER_SIZE	1024

#define PLAINTEXT 0
#define ADH	1
#define EDH	2

#define ADH_CIPHER "ADH-AES256-SHA"
//#define ADH_CIPHER "DHE-RSA-AES256-SHA"


struct io_helper {

	int listener;
	int encryption;

	BIO *connect;
	SSL_CTX *ctx;
	SSL *ssl;
	DH *dh;

	int (*remote_read)(struct io_helper *io, void *buf, size_t count);
	int (*remote_write)(struct io_helper *io, void *buf, size_t count);

};



void do_server(struct io_helper *io);
void do_client(struct io_helper *io);

int remote_read_plaintext(struct io_helper *io, void *buf, size_t count);
int remote_write_plaintext(struct io_helper *io, void *buf, size_t count);

int remote_read_encrypted(struct io_helper *io, void *buf, size_t count);
int remote_write_encrypted(struct io_helper *io, void *buf, size_t count);

void broker(struct io_helper *io);

#include "dh_params_2048.c"

int main(int argc, char **argv){


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
		io.remote_read = &remote_read_encrypted;
		io.remote_write = &remote_write_encrypted;
	}else{
		io.remote_read = &remote_read_plaintext;
		io.remote_write = &remote_write_plaintext;
	}

	printf("RAND_status(): %d\n", RAND_status());

	if(io.listener){
		do_server(&io);
	}else{
		do_client(&io);
	}

	return(0);
}

void do_server(struct io_helper *io){

//	int codes;

	BIO *accept;

	if(io->encryption > PLAINTEXT){

		if((io->ctx = SSL_CTX_new(TLSv1_server_method())) == NULL){
			fprintf(stderr, "server: SSL_CTX_new()\n");
			ERR_print_errors_fp(stderr);
			exit(-1);
		}

/*
		if((io->dh = DH_new()) == NULL){
			fprintf(stderr, "server: DH_new()\n");
			ERR_print_errors_fp(stderr);
			exit(-1);
		}

		if(!DH_generate_parameters_ex(io->dh, DH_BIT_LENGTH, DH_GENERATOR_2, NULL)){
			fprintf(stderr, "server: DH_generate_parameters_ex()\n");
			ERR_print_errors_fp(stderr);
			exit(-1);
		}

		codes = 0;
		if(!DH_check(io->dh, &codes)){
			fprintf(stderr, "server: DH_check()\n");
			ERR_print_errors_fp(stderr);
			exit(-1);
		}

		if(codes){
			fprintf(stderr, "server: codes\n");
			ERR_print_errors_fp(stderr);
			exit(-1);
		}
*/

		io->dh = get_dh2048();
/*
		if(!DH_generate_key(io->dh)){
			fprintf(stderr, "server: DH_generate_key()\n");
			ERR_print_errors_fp(stderr);
			exit(-1);
		}
*/
	
		if(!SSL_CTX_set_tmp_dh(io->ctx, io->dh)){
			fprintf(stderr, "server: SSL_CTX_set_tmp_dh()\n");
			ERR_print_errors_fp(stderr);
			exit(-1);
		}

		if(SSL_CTX_set_cipher_list(io->ctx, ADH_CIPHER) != 1){
			fprintf(stderr, "server: SSL_CTX_set_cipher_list()\n");
			ERR_print_errors_fp(stderr);
			exit(-1);
		}

	}

	if((accept = BIO_new_accept(IP ":" PORT)) == NULL){
		fprintf(stderr, "server: BIO_new_accept()\n");
		ERR_print_errors_fp(stderr);
		exit(-1);
	}

	if(BIO_set_bind_mode(accept, BIO_BIND_REUSEADDR) <= 0){
		fprintf(stderr, "server: BIO_set_bind_mode()\n");
		ERR_print_errors_fp(stderr);
		exit(-1);
	}

	if(BIO_do_accept(accept) <= 0){
		fprintf(stderr, "server: BIO_do_accept()\n");
		ERR_print_errors_fp(stderr);
		exit(-1);
	}

	if(BIO_do_accept(accept) <= 0){
		fprintf(stderr, "server: BIO_do_accept()\n");
		ERR_print_errors_fp(stderr);
		exit(-1);
	}

	if((io->connect = BIO_pop(accept)) == NULL){
		fprintf(stderr, "server: BIO_pop()\n");
		ERR_print_errors_fp(stderr);
		exit(-1);
	}

	BIO_free(accept);

	if(io->encryption > PLAINTEXT){
		if(!(io->ssl = SSL_new(io->ctx))){
			fprintf(stderr, "server: SSL_new()\n");
			ERR_print_errors_fp(stderr);
			exit(-1);
		}

		SSL_set_bio(io->ssl, io->connect, io->connect);

		if(SSL_accept(io->ssl) < 1){
			fprintf(stderr, "server: SSL_accept()\n");
			ERR_print_errors_fp(stderr);
			exit(-1);
		}

	}

	broker(io);
}


void do_client(struct io_helper *io){
	int retval;

	if(io->encryption > PLAINTEXT){

		if((io->ctx = SSL_CTX_new(TLSv1_client_method())) == NULL){
			fprintf(stderr, "client: SSL_CTX_new()\n");
			ERR_print_errors_fp(stderr);
			exit(-1);
		}

		if(SSL_CTX_set_cipher_list(io->ctx, ADH_CIPHER) != 1){
			fprintf(stderr, "client: SSL_CTX_set_cipher_list()\n");
			ERR_print_errors_fp(stderr);
			exit(-1);
		}
	}

	if((io->connect = BIO_new_connect(IP ":" PORT)) == NULL){
		fprintf(stderr, "client: BIO_new_connect()\n");
		ERR_print_errors_fp(stderr);
		exit(-1);
	}

	if(BIO_do_connect(io->connect) <= 0){
		fprintf(stderr, "client: BIO_do_connect()\n");
		ERR_print_errors_fp(stderr);
		exit(-1);
	}

	if(io->encryption > PLAINTEXT){

		if(!(io->ssl = SSL_new(io->ctx))){
			fprintf(stderr, "client: SSL_new()\n");
			ERR_print_errors_fp(stderr);
			exit(-1);
		}

		SSL_set_bio(io->ssl, io->connect, io->connect);

		if((retval = SSL_connect(io->ssl)) < 1){
			fprintf(stderr, "client: SSL_connect()\n");
			ERR_print_errors_fp(stderr);
	
			fprintf(stderr, "SSL_get_error(): %d: %s\n", SSL_get_error(io->ssl, retval), strerror(errno));
	
			exit(-1);
		}
	}

	broker(io);
}


int remote_read_plaintext(struct io_helper *io, void *buf, size_t count){
	return(BIO_read(io->connect, buf, count));
}

int remote_write_plaintext(struct io_helper *io, void *buf, size_t count){
	return(BIO_write(io->connect, buf, count));
}

int remote_read_encrypted(struct io_helper *io, void *buf, size_t count){
	return(SSL_read(io->ssl, buf, count));
}

int remote_write_encrypted(struct io_helper *io, void *buf, size_t count){
	return(SSL_write(io->ssl, buf, count));
}

void broker(struct io_helper *io){
	int retval;

	fd_set fd_select;
	int fd_max;

	char buffer[BUFFER_SIZE];

	int remote_fd;

	int char_in, char_out;

	int fcntl_flags;


	if(BIO_get_fd(io->connect, &remote_fd) < 0){
		ERR_print_errors_fp(stderr);
		exit(-1);
	}

	if((fcntl_flags = fcntl(remote_fd, F_GETFL, 0)) == -1){
		error(-1, errno, "fcntl(%d, FGETFL, 0)", remote_fd);
	}

	fcntl_flags |= O_NONBLOCK;
	if((retval = fcntl(remote_fd, F_SETFL, fcntl_flags)) == -1){
		error(-1, errno, "fcntl(%d, FSETFL, %d)", remote_fd, fcntl_flags);
	}


	while(1){

		FD_ZERO(&fd_select);
		FD_SET(STDIN_FILENO, &fd_select);
		FD_SET(remote_fd, &fd_select);

		fd_max = (STDIN_FILENO > remote_fd) ? STDIN_FILENO : remote_fd;


		if(((retval = select(fd_max + 1, &fd_select, NULL, NULL, NULL)) == -1)){
			exit(-1);
		}


		if(FD_ISSET(STDIN_FILENO, &fd_select)){

			char_in = read(STDIN_FILENO, buffer, BUFFER_SIZE);
			char_out = 0;
			while(char_in - char_out){
				char_out = io->remote_write(io, buffer + char_out, char_in - char_out);
				if(char_out < 0){
					ERR_print_errors_fp(stderr);
					exit(-1);
				}
			}

		}else if(FD_ISSET(remote_fd, &fd_select)){

			char_in = io->remote_read(io, buffer, BUFFER_SIZE);
			char_out = 0;
			while(char_in - char_out){
				char_out = write(STDOUT_FILENO, buffer + char_out, char_in - char_out);
				if(char_out < 0){
					ERR_print_errors_fp(stderr);
					exit(-1);
				}
			}
		}

	}

}
