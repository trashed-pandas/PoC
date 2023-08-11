
#include <dirent.h>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

#define	MAX_PATH_LEN	20
#define MAX_INT_LEN	10

int main(int argc, char **argv){

	int retval, pid, tty_nr, stat_fd, i;
	char path[MAX_PATH_LEN];
	DIR *target_proc_dir;
	struct dirent *dir_entry;
	char stat_char, tty_nr_char[MAX_INT_LEN + 1];	


	if(argc != 2){
		fprintf(stderr, "usage():...\n");
		exit(-1);
	}


	retval = (pid_t) strtol(argv[1], NULL, 10);
	if(errno || !retval){
		error(-1, errno, "strtol()");
	}
	pid = retval;


	memset(path, 0, sizeof(path));
	if(snprintf(path, sizeof(path), "/proc/%d/stat", pid) < 0){
		error(-1, errno, "snprintf()");
	}

	if((retval = open(path, O_RDONLY)) == -1){
		error(-1, errno, "open()");
	}
	stat_fd = retval;

	i = 0;
	tty_nr = 0;
	memset(tty_nr_char, 0, sizeof(tty_nr_char));
	while((retval = read(stat_fd, &stat_char, 1))){

		if(stat_char == ' '){
			tty_nr++;
			if(tty_nr > 6){
				break;
			}
		}

		if(tty_nr == 6){
			tty_nr_char[i++] = stat_char;
		}
	}
	if(retval == -1){
		error(-1, errno, "read()");
	}
	close(stat_fd);

	retval = strtol(tty_nr_char, NULL, 10);
	if(errno || !retval){
		error(-1, errno, "strtol()");
	}
	tty_nr = retval;

	printf("DEBUG: tty_nr: %d\n", tty_nr);

	//XXX grab stat contents, extract tty_nr

	memset(path, 0, sizeof(path));
	if(snprintf(path, sizeof(path), "/proc/%d/fd/", pid) < 0){
		error(-1, errno, "snprintf()");
	}


	if(!(target_proc_dir = opendir(path))){
		error(-1, errno, "opendir()");
	}

	while((dir_entry = readdir(target_proc_dir))){
		if(!(strcmp(dir_entry->d_name, ".") && \
					strcmp(dir_entry->d_name, ".."))){
			continue;
		}

		// XXX payload goes here.
	}


	return(0);
}
