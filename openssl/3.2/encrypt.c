
#define _POSIX_C_SOURCE 2


#include <errno.h>
#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <openssl/evp.h>
#include <openssl/rand.h>



#define STRINGIFY_VALUE(s) STRINGIFY(s)
#define STRINGIFY(s) #s

#define TEST_KEY "d9fee30e51f2361758b7decad1fc49d488eebf1ce2658703596384f830e348f0"
#define EVP_CIPHER EVP_aes_256_gcm



int decrypt(char *key);
char *encrypt();


char *program_invocation_short_name;
long pagesize = 0;

int iv_len = 0, key_len = 0;
unsigned char *in_buf = NULL, *out_buf = NULL;
unsigned char *iv_buf = NULL, *key_buf = NULL;
int iv_buf_len = 0;
long key_buf_len = 0;
EVP_CIPHER_CTX *ctx = NULL;


int main(int argc, char **argv){


	if(argc != 2){
				fprintf(stderr, "usage()...\n");
				return 1;
	}

	int opt;
	int decrypt_flag = 0;

	while ((opt = getopt(argc, argv, "d")) != -1) {
		switch(opt){
			case 'd':
				decrypt_flag = 1;
				break;
			default:
				fprintf(stderr, "usage()...\n");
				return 1;
		}
	}


	if((program_invocation_short_name = strrchr(argv[0], '/'))){
		program_invocation_short_name++;
	}else{
		program_invocation_short_name = argv[0];
	}

	pagesize = sysconf(_SC_PAGESIZE);
	iv_len = EVP_CIPHER_get_iv_length(EVP_CIPHER());
	key_len = EVP_CIPHER_get_key_length(EVP_CIPHER());
	iv_buf_len = iv_len;  // For naming convention consistency when dealing with the buffer.


	int exit_code = 0;

	if((in_buf = malloc(pagesize)) == NULL){
		fprintf(stderr, "%s: malloc(%ld): %s\n", program_invocation_short_name, pagesize, strerror(errno));
		exit_code = 1;
		goto CLEAN;
	}

	if((out_buf = malloc(pagesize)) == NULL){
		fprintf(stderr, "%s: malloc(%ld): %s\n", program_invocation_short_name, pagesize, strerror(errno));
		exit_code = 1;
		goto CLEAN;
	}

	if((iv_buf = malloc(iv_buf_len)) == NULL){
		fprintf(stderr, "%s: malloc(%d): %s\n", program_invocation_short_name, iv_buf_len, strerror(errno));
		exit_code = 1;
		goto CLEAN;
	}

	if(decrypt_flag){
		if(!decrypt(argv[1])){
			fprintf(stderr, "%s: Unable to decrypt. Quitting.\n", program_invocation_short_name);
			exit_code = 1;
			goto CLEAN;
		}
	}else{
		if(!encrypt()){
			fprintf(stderr, "%s: Unable to decrypt. Quitting.\n", program_invocation_short_name);
			exit_code = 1;
			goto CLEAN;
		}
	}


CLEAN:

	if(in_buf)
		free(in_buf);

	if(out_buf)
		free(out_buf);

	if(iv_buf)
		free(iv_buf);

	if(key_buf)
		free(key_buf);


	return exit_code;
}


int decrypt(char *key){

	key_buf = OPENSSL_hexstr2buf(key, &key_buf_len);

	return 0;
}


char *encrypt(){

	ctx = EVP_CIPHER_CTX_new();
	RAND_bytes(iv_buf, iv_buf_len);
	RAND_bytes(key_buf, key_len);
	key_buf_len = (long) key_len;

	if(key_buf_len != key_len){
		fprintf(stderr, "%s: Key supplied is not the correct length for algorithm %s: %s\n", program_invocation_short_name, STRINGIFY_VALUE(EVP_CIPHER), strerror(ENOKEY));
		return(NULL);
	}

	EVP_EncryptInit(ctx, EVP_CIPHER(), key_buf, iv_buf);

	return NULL;
}
