#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>


//#define SCRATCH "~/scratch"
#define SCRATCH "/home/empty/scratch"

int main(){
	int fd;
	char tmp_char;

	fd = open(SCRATCH, O_RDONLY);
	if(fd == -1){
		error(fd, errno, "open(%s, O_RDONLY)", SCRATCH);
	}	
	while(read(fd, &tmp_char, 1)){
		write(1, &tmp_char, 1);
	}

	return(0);
}
