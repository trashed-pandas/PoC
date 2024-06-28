#include <stdio.h>

int main(int argc, char **argv){

	int i = -1;

	
	printf("argv[%d]: %d\n", i, (int) argv[i]);

	i++;
	while(argv[i]){
		printf("argv[%d]: %s\n", i, argv[i]);

		i++;
	}

	i++;
	while(argv[i]){
		printf("env: argv[%d]: %s\n", i, argv[i]);

		i++;
	}

	i++;
	while(argv[i]){
		printf("wtf?: argv[%d]: %lx\n", i, (unsigned long) argv[i]);

		i++;
	}

	return(0);
}
