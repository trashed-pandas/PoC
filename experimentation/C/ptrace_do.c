#include "ptrace_do.h"

static void ptrace_do_free(struct ptrace_do *target);
static void print_urs(struct user_regs_struct *urs_in);

/**********************************************************************/
int ptrace_do_init(struct ptrace_do *target, int pid){
	int retval, status;

	memset(target, 0, sizeof(target));
	target->pid = pid;

#ifdef DEBUG
	printf("DEBUG: ptrace(%d, %d, %lx, %lx)\n", (int) PTRACE_ATTACH, (int) target->pid, (long unsigned int) NULL, (long unsigned int) NULL);
#endif
	ptrace(PTRACE_ATTACH, target->pid, NULL, NULL);
	if(errno){
		fprintf(stderr, "%s: ptrace(%d, %d, %lx, %lx): %s\n", program_invocation_name, \
				(int) PTRACE_ATTACH, (int) target->pid, (long unsigned int) NULL, (long unsigned int) NULL, strerror(errno));
		return(-1);
	}

#ifdef DEBUG
	printf("DEBUG: waitpid(%d, %lx, 0)\n", (int) target->pid, (unsigned long) &status);
#endif
	if((retval = waitpid(target->pid, &status, 0)) < 1){
		fprintf(stderr, "%s: waitpid(%d, %lx, 0): %s\n", program_invocation_name, \
				(int) target->pid, (unsigned long) &status, strerror(errno));
		return(-1);
	}

	if(!WIFSTOPPED(status)){
		fprintf(stderr, "%s: waitpid(%d, %lx, 0): WIFSTOPPED(%d) test failed!\n", program_invocation_name, \
				(int) target->pid, (unsigned long) &status, status);
		return(-1);
	}

#ifdef DEBUG
	printf("DEBUG: ptrace(%d, %d, %lx, %lx)\n", \
			(int) PTRACE_GETREGS, (int) target->pid, (long unsigned int) NULL, (long unsigned int) &(target->saved_regs));
#endif
	ptrace(PTRACE_GETREGS, target->pid, NULL, &(target->saved_regs));
	if(errno){
		fprintf(stderr, "%s: ptrace(%d, %d, %lx, %lx): %s\n", program_invocation_name, \
				(int) PTRACE_GETREGS, (int) target->pid, (long unsigned int) NULL, (long unsigned int) &(target->saved_regs), strerror(errno));
		return(-1);
	}

#ifdef DEBUG
	printf("DEBUG: &(target->saved_regs): %lx\n", (unsigned long) &(target->saved_regs));
	print_urs(&(target->saved_regs));
#endif

	return(0);
}

/**********************************************************************/
int ptrace_do_malloc(struct ptrace_do *target, int size){

	char *scratch;
	int i;

	union word ptrace_data;	

	scratch = (char *) malloc(size);
	if(!(scratch || size)){
		perror("ptrace_do_malloc(): malloc()");
		return(-1);
	}
	target->saved_mem_buffer = scratch;
	memset(target->saved_mem_buffer, 0, size);

	target->saved_word_count = (size + 1) / sizeof(long);
	if((size + 1) % sizeof(long)) target->saved_word_count++;

	target->address = (target->saved_regs.rsp & ~(sizeof(long) - 1)) + sizeof(long);

	for(i = 0; i < target->saved_word_count; i++){


#ifdef DEBUG
		printf("DEBUG: ptrace(%d, %d, %lx, %lx)\n", \
				(int) PTRACE_PEEKTEXT, (int) target->pid, \
				(long unsigned int) (target->address + (i * sizeof(long))), (long unsigned int) NULL);
#endif
		ptrace_data.long_word = ptrace(PTRACE_PEEKTEXT, target->pid, (void *) (target->address + (i * sizeof(long))), NULL);
		if(errno){
			fprintf(stderr, "%s: ptrace(%d, %d, %lx, %lx): %s\n", program_invocation_name, \
					(int) PTRACE_PEEKTEXT, (int) target->pid, \
					(long unsigned int) (target->address + (i * sizeof(long))), (long unsigned int) NULL, strerror(errno));
			return(-1);
		}
		memcpy(&(target->saved_mem_buffer[i * sizeof(long)]), ptrace_data.char_word, sizeof(long));
	}

	return(0);
}

/**********************************************************************/
int ptrace_do_inject_mem(struct ptrace_do *target, char *local_buffer){

	int i; 

	union word ptrace_data;

	memset(&ptrace_data, 0, sizeof(ptrace_data));

	for(i = 0; i < target->saved_word_count; i++){
		memcpy(ptrace_data.char_word, &(local_buffer[i * sizeof(long)]), sizeof(long));
#ifdef DEBUG
		printf("DEBUG: ptrace(%d, %d, %lx, %lx)\n", \
				(int) PTRACE_POKETEXT, (int) target->pid, \
				(long unsigned int) (target->address + (i * sizeof(long))), \
				(long unsigned int) ptrace_data.long_word);
#endif
		ptrace(PTRACE_POKETEXT, target->pid, (void *) (target->address + (i * sizeof(long))), (void *) ptrace_data.long_word);
		if(errno){
			fprintf(stderr, "%s: ptrace(%d, %d, %lx, %lx): %s\n", program_invocation_name, \
					(int) PTRACE_POKETEXT, (int) target->pid, \
					(long unsigned int) (target->address + (i * sizeof(long))), \
					(long unsigned int) ptrace_data.long_word, strerror(errno));
			return(-1);
		}
	}

	return(0);
}

/**********************************************************************/
unsigned long ptrace_do_syscall(struct ptrace_do *target, \
		unsigned long rax, unsigned long rdi, unsigned long rsi, unsigned long rdx){

	int retval, status;
	struct user_regs_struct attack_regs;

	memcpy(&attack_regs, &(target->saved_regs), sizeof(attack_regs));
	attack_regs.rax = rax;
	attack_regs.rdi = rdi;
	attack_regs.rsi = rsi;
	attack_regs.rdx = rdx;
	attack_regs.rip = (target->saved_regs.rip) - 2;

#ifdef DEBUG
	printf("DEBUG: &attack_regs: %lx\n", (unsigned long) &attack_regs);
	print_urs(&attack_regs);
#endif

#ifdef DEBUG
	printf("DEBUG: ptrace(%d, %d, %lx, %lx)\n", \
			(int) PTRACE_SETREGS, (int) target->pid, (long unsigned int) NULL, (long unsigned int) &attack_regs);
#endif
	ptrace(PTRACE_SETREGS, target->pid, NULL, &attack_regs);
	if(errno){
		fprintf(stderr, "%s: ptrace(%d, %d, %lx, %lx): %s\n", program_invocation_name, \
				(int) PTRACE_SETREGS, (int) target->pid, (long unsigned int) NULL, (long unsigned int) &attack_regs, strerror(errno));
		return(-1);
	}

#ifdef DEBUG
	printf("DEBUG: ptrace(%d, %d, %lx, %lx)\n", \
			(int) PTRACE_SINGLESTEP, (int) target->pid, (long unsigned int) NULL, (long unsigned int) NULL);
#endif
	ptrace(PTRACE_SINGLESTEP, target->pid, NULL, NULL);
	if(errno){
		fprintf(stderr, "%s: ptrace(%d, %d, %lx, %lx): %s\n", program_invocation_name, \
				(int) PTRACE_SINGLESTEP, (int) target->pid, (long unsigned int) NULL, (long unsigned int) NULL, strerror(errno));
		return(-1);
	}
#ifdef DEBUG
	printf("DEBUG: waitpid(%d, %lx, 0)\n", (int) target->pid, (unsigned long) &status);
#endif
	if((retval = waitpid(target->pid, &status, 0)) < 1){
		fprintf(stderr, "%s: waitpid(%d, %lx, 0): %s\n", program_invocation_name, \
				(int) target->pid, (unsigned long) &status, strerror(errno));
		return(-1);
	}

	if(!WIFSTOPPED(status)){
		fprintf(stderr, "%s: waitpid(%d, %lx, 0): WIFSTOPPED(%d) test failed!\n", program_invocation_name, \
				(int) target->pid, (unsigned long) &status, status);
		return(-1);
	}

#ifdef DEBUG
	printf("DEBUG: ptrace(%d, %d, %lx, %lx)\n", \
			(int) PTRACE_GETREGS, (int) target->pid, (long unsigned int) NULL, (long unsigned int) &attack_regs);
#endif
	ptrace(PTRACE_GETREGS, target->pid, NULL, &attack_regs);
	if(errno){
		fprintf(stderr, "%s: ptrace(%d, %d, %lx, %lx): %s\n", program_invocation_name, \
				(int) PTRACE_GETREGS, (int) target->pid, (long unsigned int) NULL, (long unsigned int) &attack_regs, strerror(errno));
		return(-1);
	}

	return(attack_regs.rax);	
}

/**********************************************************************/
int ptrace_cleanup(struct ptrace_do *target){

	union word ptrace_data;
	int i;


	for(i = 0; i < target->saved_word_count; i++){
		memcpy(ptrace_data.char_word, &(target->saved_mem_buffer[i * sizeof(long)]), sizeof(long));
#ifdef DEBUG
		printf("DEBUG: ptrace(%d, %d, %lx, %lx)\n", \
				(int) PTRACE_POKETEXT, (int) target->pid, \
				(long unsigned int) (target->address + (i * sizeof(long))), \
				(long unsigned int) ptrace_data.long_word);
#endif
		ptrace(PTRACE_POKETEXT, target->pid, (void *) (target->address + (i * sizeof(long))), (void *) ptrace_data.long_word);
		if(errno){
			fprintf(stderr, "%s: ptrace(%d, %d, %lx, %lx): %s\n", program_invocation_name, \
					(int) PTRACE_POKETEXT, (int) target->pid, \
					(long unsigned int) (target->address + (i * sizeof(long))), \
					(long unsigned int) ptrace_data.long_word, strerror(errno));
			return(-1);
		}
	}

#ifdef DEBUG
	printf("DEBUG: ptrace(%d, %d, %lx, %lx)\n", \
			(int) PTRACE_SETREGS, (int) target->pid, (long unsigned int) NULL, (long unsigned int) &(target->saved_regs));
#endif
	ptrace(PTRACE_SETREGS, target->pid, NULL, &(target->saved_regs));
	if(errno){
		fprintf(stderr, "%s: ptrace(%d, %d, %lx, %lx): %s\n", program_invocation_name, \
				(int) PTRACE_SETREGS, (int) target->pid, (long unsigned int) NULL, (long unsigned int) &(target->saved_regs), strerror(errno));
		return(-1);
	}

#ifdef DEBUG
	printf("DEBUG: ptrace(%d, %d, %lx, %lx)\n", \
			(int) PTRACE_DETACH, (int) target->pid, (long unsigned int) NULL, (long unsigned int) NULL);
#endif
	ptrace(PTRACE_DETACH, target->pid, NULL, NULL);
	if(errno){
		fprintf(stderr, "%s: ptrace(%d, %d, %lx, %lx): %s\n", program_invocation_name, \
				(int) PTRACE_DETACH, (int) target->pid, (long unsigned int) NULL, (long unsigned int) NULL, strerror(errno));
		return(-1);
	}

	ptrace_do_free(target);

	return(0);
}

/**********************************************************************/
static void ptrace_do_free(struct ptrace_do *target){

	target->address = (unsigned long) NULL;
	target->saved_word_count = 0;
	free(target->saved_mem_buffer);
}

/**********************************************************************/
#ifdef DEBUG
static void print_urs(struct user_regs_struct *urs_in){
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
