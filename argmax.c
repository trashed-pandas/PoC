#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>

int main(){

	printf("ARG_MAX: %ld\n", (long int) ARG_MAX);
	printf("sysconf(_SC_ARG_MAX): %ld\n", sysconf(_SC_ARG_MAX));

	return(0);
	
}
