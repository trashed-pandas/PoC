
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define BUF_SIZE 1024


char *DEFAULT_FORTUNE_FILE = "/usr/share/games/fortunes/ascii-art";


int main(int argc, char **argv){

	char *fortune_file;
	int fd, delim_count, entry_start, entry_stop, entry_len, retval, rand_int;
	char buf[BUF_SIZE];
	char this_char, last_char;


	fortune_file = DEFAULT_FORTUNE_FILE;

	if(argc > 1){
		if(argc != 2){
			error(-1, 0, "usage: %s FORTUNE_FILE", argv[0]);
		}

		fortune_file = argv[1];
	}

	memset(buf, '\0', BUF_SIZE);
	if((fd = open(fortune_file, O_RDONLY)) == -1){
		error(-1, errno, "open(\"%s\", O_RDONLY)", fortune_file);
	}

	delim_count = 0;
	last_char = '\0';
	while((retval = read(fd, &this_char, 1)) > 0){

		if(this_char == '%' && last_char == '\n'){
			delim_count++;
		}

		last_char = this_char;
	}
	if(retval == -1){
		error(-1, errno, "read(%d, %lx, 1)", fd, (unsigned long) &this_char);
	}

	if((retval = lseek(fd, 0, SEEK_SET)) == -1){
		error(-1, errno, "lseek(%d, 0, SEEK_SET)", fd);
	}

	srand(time(NULL));
	rand_int = rand() % delim_count; 

	entry_start = 0;
	entry_stop = 0;
	delim_count = 0;
	last_char = '\0';
	while((retval = read(fd, &this_char, 1)) > 0){

		if(entry_stop || !rand_int){
			entry_stop++;
		}else{
			entry_start++;
		}

		if(this_char == '%' && last_char == '\n'){
			delim_count++;
			if(delim_count == rand_int){
				entry_stop = entry_start;
			}else if(entry_stop){
				break;
			}
		}

		last_char = this_char;
	}
	if(retval == -1){
		error(-1, errno, "read(%d, %lx, 1)", fd, (unsigned long) &this_char);
	}

	if(rand_int){
		entry_start++;
	}
	entry_stop--;
	entry_len = entry_stop - entry_start;
	if((retval = lseek(fd, entry_start, SEEK_SET)) == -1){
		error(-1, errno, "lseek(%d, %d, SEEK_SET)", fd, entry_start);
	}

	entry_len = (entry_len > (BUF_SIZE - 1)) ? (BUF_SIZE - 1) : entry_len;
	memset(buf, '\0', BUF_SIZE);

	if((retval = read(fd, buf, entry_len)) == -1){
		error(-1, errno, "read(%d, %lx, %d)", fd, (unsigned long) &buf, entry_len);
	}

	entry_len = retval;
	if((retval = write(STDOUT_FILENO, buf, entry_len)) == -1){
		error(-1, errno, "write(%d, %lx, %d)", STDOUT_FILENO, (unsigned long) &buf, entry_len);
	}

	return(0);
}
