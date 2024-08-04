#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv){

	int i;
	
	for(i = 0; i < argc; i++){
		printf("argv[%d]: %s : %ld\n", i, argv[i], strtol(argv[i], NULL, 16));
	}	

	return(0);
}
