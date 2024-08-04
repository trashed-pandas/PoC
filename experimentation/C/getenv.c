#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	int i;	
	char *value;

	i = 1;
	while(argv[i]){
		value = getenv(argv[i]);
		printf("%s -> %s\n", argv[i], value);
		i++;
	}	

	return(0);
}
