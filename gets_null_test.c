#include <stdio.h>
#include <string.h>

#define BUFF_LEN 32

int main(){

	int i;
	char buffer[BUFF_LEN];

	printf("before initialization:\n\t");
	for(i = 0; i < BUFF_LEN; i++){
		printf("\\x%02x", buffer[i]);
	}

	memset(buffer, '\0', BUFF_LEN);

	printf("\nafter initialization:\n\t");
	for(i = 0; i < BUFF_LEN; i++){
		printf("\\x%02x", buffer[i]);
	}
	
	printf("\nEnter input: ");	
	gets(buffer);
	
	printf("after gets():\n\t");
	for(i = 0; i < BUFF_LEN; i++){
		printf("\\x%02x", buffer[i]);
	}
	printf("\n");

	return(0);
}
