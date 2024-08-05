#include <stdio.h>
#include <unistd.h>


// check the access this user has for the files in argv
int main(int argc, char **argv){

	int i = 1;

	while(i < argc){

		printf("%s:", argv[i]);

		// exists?
		if(!access(argv[i], F_OK))
			printf(" exists");

		// can read?
		if(!access(argv[i], R_OK))
			printf(" read");

		// can write?
		if(!access(argv[i], W_OK))
			printf(" write");

		// can execute?
		if(!access(argv[i], X_OK))
			printf(" execute");

		printf("\n");
		i++;
	}
	
	return(0);
}
