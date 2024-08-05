#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>

// what is the max number of arguments that can be passed through execve()
// this is a good example of sysconf()
int main(){

	printf("ARG_MAX: %ld\n", (long int) ARG_MAX);
	printf("sysconf(_SC_ARG_MAX): %ld\n", sysconf(_SC_ARG_MAX));

	return(0);
	
}
