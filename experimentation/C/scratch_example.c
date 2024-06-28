#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
	
	char *scratch;
	int buff_size;

	buff_size = getpagesize();
	scratch = (char *) calloc(buff_size, sizeof(char));

	read(STDIN_FILENO, scratch, buff_size - 1);

	printf("scratch: %s", scratch);

	return(0);
}
