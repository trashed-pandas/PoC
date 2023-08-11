
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>


int main(){

	printf("before:\n");
	printf("pid: %d\n", getpid());
	printf("pgid: %d\n", getpgid(0));

	if(fork()){
		return(0);
	}	

	printf("after:\n");
	printf("pid: %d\n", getpid());
	printf("pgid: %d\n", getpgid(0));
	printf("setsid(): %d\n", (int) setsid());
}
