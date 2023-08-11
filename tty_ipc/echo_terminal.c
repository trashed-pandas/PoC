#define _XOPEN_SOURCE 600

#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 256

int main(int argc, char **argv){
	
	char scratch[BUFFER_SIZE];

	int retval, tmp, i;
	int fd[2];

	fd_set read_fds;

	if(argc != 2){
		fprintf(stderr, "usage(): ...\n");
		exit(-1);
	}

	if((retval = open(argv[1], O_RDWR)) == -1){
		error(-1, errno, "open()");
	}
	fd[0] = retval;
	
	if((retval = posix_openpt(O_RDWR)) == -1){
		error(-1, errno, "posix_openpt()");
	}
	fd[1] = retval;

	if((retval = grantpt(fd[1])) == -1){
		error(-1, errno, "grantpt()");
	}

	if((retval = unlockpt(fd[1])) == -1){
		error(-1, errno, "grantpt()");
	}

	printf("DEBUG: ptsname(%d): %s\n", fd[1], ptsname(fd[1]));
/*
	printf("DEBUG: sleeping for 10.\n");
	sleep(10);
	printf("DEBUG: awake and proceeding.\n");
*/

	while(1){
		FD_ZERO(&read_fds);
		FD_SET(fd[0], &read_fds);
		FD_SET(fd[1], &read_fds);
		
		if((retval = select(fd[1] + 1, &read_fds, NULL, NULL, NULL)) == -1){
			error(-1, errno, "select()");
		}

		for(i = 0; i < 2; i++){	
			if(FD_ISSET(fd[i], &read_fds)){

				printf("DEBUG: i: %d\n", i);

				memset(scratch, 0, sizeof(scratch));
				if((retval = read(fd[i], &scratch, sizeof(scratch))) == -1){
					error(-1, errno, "read()");
				}
				tmp = retval;
				
				if((retval = write(fd[i ^ 1], &scratch, tmp)) == -1){
					error(-1, errno, "write()");
				}

				if((retval = write(1, &scratch, tmp)) == -1){
					error(-1, errno, "write()");
				}
			}
		}
	}

	return(0);
}
