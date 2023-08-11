#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <unistd.h>

#define BUFF_LEN 32

int main(){

	char buffer[BUFF_LEN];

	if(fgets(buffer, BUFF_LEN, stdin) == NULL){
		error(-1, errno, "fgets(%p, %d, %p)", (void *) buffer, BUFF_LEN, (void *) stdin);
	}

	printf("buffer: %s\n", buffer);
	pause();
	return(0);
}
