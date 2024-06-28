#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
	
	printf("uid: %d\n", getuid());
	printf("euid: %d\n", geteuid());

	return(0);
}
