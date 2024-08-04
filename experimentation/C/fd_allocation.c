#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(){

	int fd_a, fd_b, fd_c;

	fd_a = open("/tmp/alpha", O_RDONLY);
	printf("fd_a: %d\n", fd_a);	
	fd_b = open("/tmp/bravo", O_RDONLY);
	printf("fd_b: %d\n", fd_b);	
	fd_c = open("/tmp/charlie", O_RDONLY);
	printf("fd_c: %d\n", fd_c);	
	close(fd_a);
	fd_c = open("/tmp/delta", O_RDONLY);
	printf("fd_a: %d\n", fd_a);	

	return(0);
}
