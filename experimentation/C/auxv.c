#include <stdio.h>

// exploring the stack's high address space. This works because argv, envp, and auxv vectors are implemented as NULL terminated themselves.

// yes, this works and is interesting, but maybe:
//    man 3 getauxval

// This code will throw warnings, which I never like, but lets be honest, this entire snippet is unholy as hell and I'd worry if the compiler _didn't_ complain

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
