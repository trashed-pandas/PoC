#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024

void enc(char *buffer, int length);
void dec(char *buffer, int length);

int main(int argc, char **argv){

	char buffer[BUF_SIZE];
	int length;

	if(argc != 2){
		exit(-1);
	}

	memset(buffer, 0, BUF_SIZE);	
	length = strlen(argv[1]);
	memcpy(buffer, argv[1], BUF_SIZE);
	
	
//	printf("%s\n", buffer);

	enc(buffer, BUF_SIZE);
	printf("%s", buffer);

//	dec(buffer, length);
//	printf("%s\n", buffer);

	return(0);
}

void enc(char *buffer, int length){

	unsigned int key;
	int i;

	key = length & 0xff;

	for(i = 0; i < length; i++) {
		buffer[i] ^= key;
		key -= buffer[i] ^ key;
	}
}

void dec(char *buffer, int length){

	unsigned int key;
	int i;

	key = length & 0xff;

	for(i = 0; i < length; i++) {
		buffer[i] ^= key;
		key -= buffer[i];
	}
}
