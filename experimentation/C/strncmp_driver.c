#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
	int num_offset = 8;
	char *strtol_ptr;
	int tty_num;

	if(argc != 2){
		fprintf(stderr, "usage()\n");
		exit(-1);
	}

	if(!strncmp("/dev/tty", argv[1], 8)){
		printf("/dev/tty* found!\n");
	}else if(!strncmp("/dev/pts/", argv[1], 9)){
		printf("/dev/pts/* found!\n");
		num_offset++;
	}else{
		printf("No match found.\n");
		exit(-1);
	}

	tty_num = strtol(&(argv[1][num_offset]), &strtol_ptr, 10);

	if(errno){
		fprintf(stderr, "%s is causing problems: %s.\n", argv[1], strerror(errno));
	}else if(*strtol_ptr){
		fprintf(stderr, "%s is not a number.\n", &(argv[1][num_offset]));
	}else{
		printf("tty_num: %d\n", tty_num);
	}

	return(0);
}
