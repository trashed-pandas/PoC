#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char **argv){
	int i;

	if(argc == 2){
		i = strtol(argv[1], NULL, 16);
		printf("chmod(\"/tmp/X\", 0x%x)\n", i); 
		chmod("/tmp/X", i);
		return(0);
	}

	for(i = 0; i > -1; i++){
		errno = 0;
		chmod("/tmp/X", i);
		printf("%04x: %s\n", i, strerror(errno));
	}

	return(0);
}
