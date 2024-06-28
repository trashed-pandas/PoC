#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PR_SET_MM		35
#define	PR_SET_MM_EXE_FILE	13

int main(){

	int retval;
	int fd;

	char *binary = "/bin/false";

	if((fd = open(binary, O_RDONLY)) == -1){
		fprintf(stderr, "%d: open(\"/bin/false\", O_RDONLY): %s\n", fd, strerror(errno));
		goto PAUSE;
	}

	if((retval = prctl(PR_SET_NAME, binary, 0, 0, 0))){
		fprintf(stderr, "%d: prctrl(%d, %lx, 0, 0, 0): %s\n", retval, PR_SET_NAME, (unsigned long) binary, strerror(errno));
		goto PAUSE;
	}

	if((retval = prctl(PR_SET_MM, PR_SET_MM_EXE_FILE, fd, 0, 0))){
		fprintf(stderr, "%d: prctrl(%d, %d, %d, 0, 0): %s\n", retval, PR_SET_MM, PR_SET_MM_EXE_FILE, fd, strerror(errno));
		goto PAUSE;
	}

PAUSE:
	pause();

	return(0);
}
