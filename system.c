#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){

	char line[1024];

	strcpy(line, "-p ");
	strcpy(line + 3, argv[1]);
	printf("DEBUG: line: %s\n", line);
	system(argv[1]);
	printf("after.\n");

	return(0);
}
