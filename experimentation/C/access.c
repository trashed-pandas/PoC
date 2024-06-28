#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv){
	int i;
	int retval;

	i = 0;
	while(argv[i]){
		printf("argv[%d]: %s\n", i, argv[i]);
		
		retval = 0;
		errno = 0;
		//retval = access(argv[i], F_OK);
		retval = access(argv[i], NULL);

		printf("variable -> %s\n", variable);

		printf("\t%d: %d\n", retval, errno);
			
		i++;
	}
	
	return(0);
}
