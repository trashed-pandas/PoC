#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>


int main(int argc, char **argv){

	int retval;
	int fd; 
	struct termios termios_attrs;

/*
	if(argc != 2){
		fprintf(stderr, "usage(): ...\n");
		exit(-1);
	}	

	if((retval = open(argv[1], O_RDONLY)) == -1){
		error(-1, errno, "open(%s, O_RDONLY)", argv[1]);
	}
	fd = retval;
*/

//	if((retval = tcgetattr(fd, &termios_attrs)) == -1){
	if((retval = tcgetattr(STDIN_FILENO, &termios_attrs)) == -1){
		error(-1, errno, "tcgetattr(%d, %lx)", STDIN_FILENO, (unsigned long) &termios_attrs);
	}

	char *cc_chars[] = { \
		"VINTR", \
		"VQUIT", \
		"VERASE", \
		"VKILL", \
		"VEOF", \
		"VTIME", \
		"VMIN", \
		"VSWTC", \
		"VSTART", \
		"VSTOP", \
		"VSUSP", \
		"VEOL", \
		"VREPRINT", \
		"VDISCARD", \
		"VWERASE", \
		"VLNEXT", \
		"VEOL2", \
	};

	int i;
	for(i = 0; i < 17; i++){
		printf("%s: 0x%02x\n", cc_chars[i], termios_attrs.c_cc[i]);
	}

	/*
		 printf("&termios_attrs: %lx\n", (unsigned long) &termios_attrs);
		 printf("termios_attrs.c_iflag: %lx\n", (unsigned long) termios_attrs.c_iflag);
		 printf("termios_attrs.c_oflag: %lx\n", (unsigned long) termios_attrs.c_oflag);
		 printf("termios_attrs.c_cflag: %lx\n", (unsigned long) termios_attrs.c_cflag);
		 printf("termios_attrs.c_lflag: %lx\n", (unsigned long) termios_attrs.c_lflag);
		 printf("termios_attrs.c_lflag & ECHO: %d\n", (int) (termios_attrs.c_lflag & ECHO));

	//termios_attrs.c_lflag &= ~ECHO;
	termios_attrs.c_lflag |= ECHO;
	if((retval = tcsetattr(fd, TCSANOW, &termios_attrs)) == -1){
	error(-1, errno, "tcgetattr(%d, %lx)", fd, (unsigned long) &termios_attrs);
	}

	printf("&termios_attrs: %lx\n", (unsigned long) &termios_attrs);
	printf("termios_attrs.c_iflag: %lx\n", (unsigned long) termios_attrs.c_iflag);
	printf("termios_attrs.c_oflag: %lx\n", (unsigned long) termios_attrs.c_oflag);
	printf("termios_attrs.c_cflag: %lx\n", (unsigned long) termios_attrs.c_cflag);
	printf("termios_attrs.c_lflag: %lx\n", (unsigned long) termios_attrs.c_lflag);
	printf("termios_attrs.c_lflag & ECHO: %d\n", (int) (termios_attrs.c_lflag & ECHO));

	sleep(30);

	termios_attrs.c_lflag |= ECHO;
	if((retval = tcsetattr(fd, TCSANOW, &termios_attrs)) == -1){
	error(-1, errno, "tcgetattr(%d, %lx)", fd, (unsigned long) &termios_attrs);
	}
	 */
}
