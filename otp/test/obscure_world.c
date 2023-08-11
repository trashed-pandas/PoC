#include <stdio.h>

int main(){
	int i;
	
	char cipher[] = "\x34\x6a\x85\x5a\x4a\x21\x3a\x2b\x3a\x34\xe3\x5a\x46\x79";
	char key[] = "\x7c\x0f\xe9\x36\x25\x01\x4d\x44\x48\x58\x87\x7b\x4b\x73";

	for(i = 0; i < (int) sizeof(cipher); i++){
		printf("%c", cipher[i] ^ key[i]);
	}
	
	return(0);
}
