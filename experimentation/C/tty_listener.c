#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int	main(int argc, char **argv){
	int tty_fd;
	char char_buf;

	if(argc != 2){
		fprintf(stderr, "%s: usage(): ...\n", argv[0]);
		exit(-1);
	}
	
	errno = 0;
	tty_fd = open(argv[1], O_RDONLY);
	if(errno){
		error(-1, errno, "open(%s, O_RDONLY)", argv[1]);
	}

	while(read(tty_fd, &char_buf, 1)){
		write(1, &char_buf, 1);
	}

	close(tty_fd);
	exit(0);
}
