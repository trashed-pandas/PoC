#define _BSD_SOURCE

#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
	int retval;

	struct stat test;

	if((retval = fstat(STDIN_FILENO, &test)) == -1){
		error(-1, errno, "fstat(STDIN_FILENO, %lx)", (unsigned long) &test);
	}

	printf("&test: %lx\n", (unsigned long) &test);
	printf("test.st_dev: %lx\n", test.st_dev);
	printf("test.st_ino: %lx\n", test.st_ino);
	printf("test.st_mode: %lx\n", (unsigned long) test.st_mode);
	printf("test.st_nlink: %lx\n", test.st_nlink);
	printf("test.st_uid: %lx\n", (unsigned long) test.st_uid);
	printf("test.st_gid: %lx\n", (unsigned long) test.st_gid);
	printf("test.st_rdev: %lx\n", test.st_rdev);
	printf("test.st_size: %lx\n", test.st_size);
	printf("test.st_blksize: %lx\n", test.st_blksize);
	printf("test.st_blocks: %lx\n", test.st_blocks);
	printf("test.st_atime: %lx\n", test.st_atime);
	printf("test.st_mtime: %lx\n", test.st_mtime);
	printf("test.st_ctime: %lx\n", test.st_ctime);

	printf("\nmajor(test.st_dev): %lx\n", (unsigned long) major(test.st_dev));
	printf("minor(test.st_dev): %lx\n", (unsigned long) minor(test.st_dev));

	return(0);
}
