
#define _XOPEN_SOURCE 600
#define _BSD_SOURCE

#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/stat.h>

#define BUFFER_SIZE 256
#define TIME_BUFFER_SIZE 16

int main(){

	char scratch[BUFFER_SIZE];
	char hctarcs[BUFFER_SIZE];
	char time_buffer[TIME_BUFFER_SIZE];
	char *retval_str;

	int retval;
	int read_count;
	int tty_fd;
	int fd[2];
	int i, j;
	int bytes_read;

	fd_set read_fds;
	struct termios termios_attrs;

	if((retval = posix_openpt(O_RDWR)) == -1){
		error(-1, errno, "posix_openpt()");
	}
	tty_fd = retval;

	if((retval = grantpt(tty_fd)) == -1){
		error(-1, errno, "grantpt()");
	}

	if((retval = unlockpt(tty_fd)) == -1){
		error(-1, errno, "unlockpt()");
	}

	if(!(retval_str = ptsname(tty_fd))){
		error(-1, errno, "ptsname()");
	}
	memcpy(scratch, retval_str, sizeof(scratch));	


	if((retval = fork()) == -1){
		error(-1, errno, "fork()");
	}

	if(retval){
		/******************************************/
		/* parent */
		/******************************************/
	
		fd[0] = 0;
		fd[1] = tty_fd;

		while(1){
			FD_ZERO(&read_fds);
			FD_SET(fd[0], &read_fds);
			FD_SET(fd[1], &read_fds);

			if((retval = select(tty_fd + 1, &read_fds, NULL, NULL, NULL)) == -1){
				error(-1, errno, "select()");
			}

			printf("DEBUG: select(): retval: %d\n", retval);
      for(i = 0; i < 2; i++){
        if(FD_ISSET(fd[i], &read_fds)){

          memset(&scratch, 0, sizeof(scratch));
          if((retval = read(fd[i], scratch, sizeof(scratch))) == -1){
            error(-1, errno, "read()");
          }
          bytes_read = retval;

          if((retval = write(fd[i ^ 1], scratch, bytes_read)) == -1){
            error(-1, errno, "write()");
          }

          // XXX still double printing when echo is on.
//          if((retval = write(1, scratch, bytes_read)) == -1){
//            error(-1, errno, "write()");
//          }
        }
      }
/*
			if(FD_ISSET(fd[0], &read_fds)){
				memset(scratch, 0, sizeof(scratch));
				if((retval = read(fd[0], scratch, sizeof(scratch))) == -1){
					error(-1, errno, "read()");
				}
				read_count = retval;

				if((retval = write(fd[1], scratch, read_count)) == -1){
					error(-1, errno, "write()");
				}
			}

			if(FD_ISSET(fd[1], &read_fds)){
				memset(scratch, 0, sizeof(scratch));
				if((retval = read(fd[1], scratch, sizeof(scratch))) == -1){
					error(-1, errno, "read()");
				}
				read_count = retval;

				if((retval = write(fd[0], scratch, read_count)) == -1){
					error(-1, errno, "write()");
				}
			}
*/
		}
	}else{

		/******************************************/
		/* child */
		/******************************************/
		if((retval = close(tty_fd)) == -1){
			error(-1, errno, "close()");
		}

		if((retval = open(scratch, O_RDWR)) == -1){
			error(-1, errno, "open()");
		}
		tty_fd = retval;

		if((retval = tcgetattr(tty_fd, &termios_attrs)) == -1){
			error(-1, errno, "tcgetattr()");
		}
		cfmakeraw(&termios_attrs);
		if((retval = tcsetattr(tty_fd, TCSANOW, &termios_attrs)) == -1){
			error(-1, errno, "tcsetattr()");
		}

		if((retval = dup2(tty_fd, 0)) == -1){
			error(-1, errno, "dup2()");
		}
		if((retval = dup2(tty_fd, 1)) == -1){
			error(-1, errno, "dup2()");
		}

		/*
			 if((retval = dup2(tty_fd, 2)) == -1){
			 error(-1, errno, "dup2()");
			 }
		 */

		if((retval = close(tty_fd)) == -1){
			error(-1, errno, "close()");
		}

		if((retval = setsid()) == -1){
			error(-1, errno, "setsid()");
		}

		if((retval = ioctl(0, TIOCSCTTY, 1)) == -1){
			error(-1, errno, "setsid()");
		}


		while(1){
			FD_ZERO(&read_fds);
			FD_SET(0, &read_fds);
			if((retval = select(1, &read_fds, NULL, NULL, NULL)) == -1){
				error(-1, errno, "select()");
			}	

			if(FD_ISSET(0, &read_fds)){
				memset(scratch, 0, sizeof(scratch));
				if((retval = read(0, scratch, sizeof(scratch))) == -1){
					error(-1, errno, "read()");
				}
				read_count = retval;

//				fprintf(stderr, "DEBUG: child: read(0, %s): %d\n", scratch, retval);

//				memset(time_buffer, 0, sizeof(time_buffer));
//				sprintf(time_buffer, "%d: ", (int) time(NULL));
//				if((retval = write(1, time_buffer, strlen(time_buffer))) == -1){
//					error(-1, errno, "write()");
//				}
//				fprintf(stderr, "DEBUG: child: write(1, %s): %d\n", time_buffer, retval);
				
				for(j = strlen(scratch) - 1; j >= 0; j--){
					hctarcs[j] = scratch[strlen(scratch) - 1 - j];
				}

				if((retval = write(1, hctarcs, read_count)) == -1){
					error(-1, errno, "write()");
				}
//				fprintf(stderr, "DEBUG: child: write(1, %s): %d\n", scratch, retval);
			}
		}
	}

	return(0);
}
