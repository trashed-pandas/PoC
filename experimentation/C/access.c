#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv){

	int i = 1;

	while(i < argc){

		printf("%s:", argv[i]);

		if(!access(argv[i], F_OK))
			printf(" exists");

		if(!access(argv[i], R_OK))
			printf(" read");

		if(!access(argv[i], W_OK))
			printf(" write");

		if(!access(argv[i], X_OK))
			printf(" execute");

		printf("\n");
		i++;
	}
	
	return(0);
}
