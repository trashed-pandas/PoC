#define _POSIX_C_SOURCE 2

#include <errno.h>
#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>

#include <openssl/evp.h>
#include <openssl/rand.h>

#define TEST_KEY "1d312ed60dc25aee195969bfa60cf68a"


int main(int argc, char **argv){

	int key_len = EVP_CIPHER_get_key_length(EVP_aes_256_gcm());
	int iv_len = EVP_CIPHER_get_iv_length(EVP_aes_256_gcm());

	printf("key_len: %d\n", key_len);
	printf("iv_len: %d\n", iv_len);
	exit(0);

	int opt;
	int decrypt_flag = 0;

	unsigned char *in_buf, *out_buf;
	//unsigned char *iv_buf = NULL;
	unsigned char iv_buf[12];
	unsigned char *key_buf = NULL;

	long pagesize = sysconf(_SC_PAGESIZE);


	long key_buf_len = 0;
	key_buf = OPENSSL_hexstr2buf(TEST_KEY, &key_buf_len);

	
	while ((opt = getopt(argc, argv, "d")) != -1) {
		switch(opt){
			case 'd':
				decrypt_flag = 1;
				break;
			default:
				fprintf(stderr, "usage()...\n");
		}
	}

	if((in_buf = malloc(pagesize)) == NULL)
		error(1, errno, "malloc(%ld)", pagesize);
	
	if((out_buf = malloc(pagesize)) == NULL)
		error(1, errno, "malloc(%ld)", pagesize);
	

	printf("hello, ");
	if(decrypt_flag){
		printf("decrypt");
	}else{
		printf("encrypt");
	}
	printf("\n");


	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

	printf("iv_buf: %p\n", (void *) iv_buf);
	printf("key_buf: %p\n", (void *) key_buf);
	RAND_bytes(iv_buf, 12);
	int retval = EVP_EncryptInit(ctx, EVP_aes_256_gcm(), key_buf, iv_buf);
	printf("retval: %d\n", retval);
	perror(NULL);

	printf("iv_buf: %p\n", (void *) iv_buf);
	printf("iv_buf: %s\n", iv_buf);
	printf("key_buf: %p\n", (void *) key_buf);
	printf("key_buf: %s\n", key_buf);

CLEAN:

	free(in_buf);
	free(out_buf);
	free(key_buf);


	return 0;
}
