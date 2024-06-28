#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>
#include <fcntl.h>

int main(){

	int fcntl_flags;	

	/* Set the socket to non-blocking. */
	if((fcntl_flags = fcntl(STDIN_FILENO, F_GETFL, 0)) == -1){
		fprintf(stderr, "fcntl(%d, FGETFL, 0): %s", STDIN_FILENO, strerror(errno));
		return(-1);
	}

	if(fcntl_flags & O_NONBLOCK){
		printf("STDIN_FILENO: O_NONBLOCK\n");
	}else{
		printf("STDIN_FILENO: !O_NONBLOCK\n");
	}


	fcntl_flags |= O_NONBLOCK;
	if(fcntl(STDIN_FILENO, F_SETFL, fcntl_flags) == -1){
		fprintf(stderr, "fcntl(%d, FGETFL, %d): %s", STDIN_FILENO, fcntl_flags, strerror(errno));
		return(-1);
	}

	if(fcntl_flags & O_NONBLOCK){
		printf("STDIN_FILENO: O_NONBLOCK\n");
	}else{
		printf("STDIN_FILENO: !O_NONBLOCK\n");
	}

	if((fcntl_flags = fcntl(STDOUT_FILENO, F_GETFL, 0)) == -1){
		fprintf(stderr, "fcntl(%d, FGETFL, 0): %s", STDOUT_FILENO, strerror(errno));
		return(-1);
	}

	if(fcntl_flags & O_NONBLOCK){
		printf("STDOUT_FILENO: O_NONBLOCK\n");
	}else{
		printf("STDOUT_FILENO: !O_NONBLOCK\n");
	}

	if((fcntl_flags = fcntl(STDERR_FILENO, F_GETFL, 0)) == -1){
		fprintf(stderr, "fcntl(%d, FGETFL, 0): %s", STDIN_FILENO, strerror(errno));
		return(-1);
	}

	if(fcntl_flags & O_NONBLOCK){
		printf("STDERR_FILENO: O_NONBLOCK\n");
	}else{
		printf("STDERR_FILENO: !O_NONBLOCK\n");
	}
	return(0);
}
