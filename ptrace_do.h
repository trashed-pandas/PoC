#define DEBUG

#define _GNU_SOURCE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>

/* declare our data structures */
union word{
	long long_word;
	char char_word[sizeof(long)];
};

struct ptrace_do{
	int pid;
	struct user_regs_struct saved_regs;

	unsigned long address;
	int saved_word_count;
	char *saved_mem_buffer;
};

int ptrace_do_init(struct ptrace_do *target, int pid);
int ptrace_do_malloc(struct ptrace_do *target, int size);
int ptrace_do_inject_mem(struct ptrace_do *target, char *local_buffer);
unsigned long ptrace_do_syscall(struct ptrace_do *target, \
		unsigned long rax, unsigned long rdi, unsigned long rsi, unsigned long rdx);
int ptrace_cleanup(struct ptrace_do *target);
