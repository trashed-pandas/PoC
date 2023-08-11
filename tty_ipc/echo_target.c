#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 256
#define TIME_SIZE 16

int main(int argc, char **argv){

	char buffer[BUFFER_SIZE];
	char time_buffer[TIME_SIZE];

	int retval, i;
	int terminal_fd;


	if(argc != 2){
		fprintf(stderr, "usage(): ...\n");
		exit(-1);
	}

	if((retval = open(argv[1], O_RDWR)) == -1){
		exit(-2);
	}
	terminal_fd = retval;

	if((retval = dup2(0, terminal_fd)) == -1){
		exit(-3);
	}

	if((retval = dup2(1, terminal_fd)) == -1){
		exit(-4);
	}

	if((retval = dup2(2, terminal_fd)) == -1){
		exit(-5);
	}

	close(terminal_fd);

	i = 0;
	memset(time_buffer, 0, sizeof(time_buffer));
	memset(buffer, 0, sizeof(buffer));
	while(1){
		if((retval = read(0, buffer, sizeof(buffer)) == -1)){
			exit(-6);
		}
	
		i += retval;	

		if(buffer[i] == '\r'){
			buffer[i + 1] = '\n';
			sprintf(time_buffer, "%d: ", (int) time(NULL));

			if((retval = write(1, time_buffer, strlen(time_buffer))) == -1){
				exit(-7);
			}

			if((retval = write(1, buffer, strlen(buffer))) == -1){
				exit(-8);
			}

			i = 0;
			memset(time_buffer, 0, sizeof(time_buffer));
			memset(buffer, 0, sizeof(buffer));
		}else if(i == (sizeof(buffer) - 2)){
			exit(-9);
		}else{
			i++;
		}
	}

	return(0);
}
