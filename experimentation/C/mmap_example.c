#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main(){
	char *passwd;

	char *passwd_path = "/etc/passwd";

	int fd = open(passwd_path, O_RDONLY);

	if((passwd = (char *) mmap(NULL, 1024, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED){
		error(-1, errno, "mmap()");	
	}

	printf("%s", passwd);

	pause();
}
