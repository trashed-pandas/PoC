

#include <errno.h>
#include <stdio.h>
#include <string.h>


int main(){

	int i;

	for(i = 0; i <= ENOTRECOVERABLE; i++){
		printf("%d: %s\n", i, strerror(i));
	}

	return(0);
}
