#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 256

int main(int argc, char **argv){

	char buffer[BUFFER_SIZE];

	while(1){
		memset(buffer, 0, sizeof(buffer));
		errno = 0;
		if(!fgets(buffer, sizeof(buffer), stdin)){	
			error(-1, errno, "fgets()");
		}

		printf("%d: %s\n", (int) time(NULL), buffer);
		sleep(2);
	}

	return(0);
}
