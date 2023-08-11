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

#define EXIT_CODE	42

void ptrace_do(enum __ptrace_request request, pid_t pid, void *addr, void *data);
void waitpid_do(pid_t pid, char *program_name);
void print_urs(struct user_regs_struct *urs_in);

int main(int argc, char **argv){

	pid_t target_pid;
	struct user_regs_struct saved_regs, attack_regs;


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

	ptrace_do(PTRACE_ATTACH, target_pid, NULL, NULL);
	waitpid_do(target_pid, argv[0]);

	memset(&saved_regs, 0, sizeof(saved_regs));
	ptrace_do(PTRACE_GETREGS, target_pid, NULL, &saved_regs);

//	printf("DEBUG: print_urs(&saved_regs)\n");
//	print_urs(&saved_regs);

	memcpy(&attack_regs, &saved_regs, sizeof(attack_regs));
	attack_regs.rax = __NR_exit;
	attack_regs.rdi = EXIT_CODE;
	attack_regs.rip = saved_regs.rip - 2;

//	printf("DEBUG: print_urs(&attack_regs)\n");
//	print_urs(&attack_regs);

	ptrace_do(PTRACE_SETREGS, target_pid, NULL, &attack_regs);

	ptrace_do(PTRACE_SINGLESTEP, target_pid, NULL, NULL);
	waitpid_do(target_pid, argv[0]);

	ptrace_do(PTRACE_DETACH, target_pid, NULL, NULL);

	return(0);
}

void ptrace_do(enum __ptrace_request request, pid_t pid, void *addr, void *data){
  printf("ptrace(%d, %d, %lx, %lx)\n", (int) request, (int) pid, (long unsigned int) addr, (long unsigned int) data);
  errno = 0;
  ptrace(request, pid, addr, data);
  if(errno){
    error(-1, errno, "ptrace(%d, %d, %lx, %lx)\n", (int) request, (int) pid, (long unsigned int) addr, (long unsigned int) data);
  }
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

void print_urs(struct user_regs_struct *urs_in){
	printf("&urs_in->%lx\n", (long int) &urs_in);
	printf("	r15->%lx\n", urs_in->r15);
	printf("	r14->%lx\n", urs_in->r14);
	printf("	r13->%lx\n", urs_in->r13);
	printf("	r112->%lx\n", urs_in->r12);
	printf("	rbp->%lx\n", urs_in->rbp);
	printf("	rbx->%lx\n", urs_in->rbx);
	printf("	r11->%lx\n", urs_in->r11);
	printf("	r10->%lx\n", urs_in->r10);
	printf("	r9->%lx\n", urs_in->r9);
	printf("	r8->%lx\n", urs_in->r8);
	printf("	rax->%lx\n", urs_in->rax);
	printf("	rcx->%lx\n", urs_in->rcx);
	printf("	rdx->%lx\n", urs_in->rdx);
	printf("	rsi->%lx\n", urs_in->rsi);
	printf("	rdi->%lx\n", urs_in->rdi);
	printf("	orig_rax->%lx\n", urs_in->orig_rax);
	printf("	rip->%lx\n", urs_in->rip);
	printf("	rcs->%lx\n", urs_in->cs);
	printf("	eflags->%lx\n", urs_in->eflags);
	printf("	rsp->%lx\n", urs_in->rsp);
	printf("	ss->%lx\n", urs_in->ss);
	printf("	fs_base->%lx\n", urs_in->fs_base);
	printf("	gs_base->%lx\n", urs_in->gs_base);
	printf("	ds->%lx\n", urs_in->ds);
	printf("	es->%lx\n", urs_in->es);
	printf("	fs->%lx\n", urs_in->fs);
	printf("	gs->%lx\n", urs_in->gs);
	printf("\n");
}
