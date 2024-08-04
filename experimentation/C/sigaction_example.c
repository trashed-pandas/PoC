
#define _GNU_SOURCE

#include <errno.h>
#include <error.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t sig_found = 0;


void sig_handler(int signal);


void signal_handler(int signal){
	sig_found = signal;
}


int main(){
	int retval, i;
	struct sigaction act;
	sigset_t all_sigs;

	memset(&act, 0, sizeof(act));

	act.sa_handler = signal_handler;

	sigfillset(&all_sigs);
	for(i = 1; i < NSIG; i++){
		if(sigismember(&all_sigs, i) && !((i == SIGKILL) || (i == SIGSTOP))){
			if((retval = sigaction(i, &act, NULL)) == -1){
				error(-1, errno, "sigaction(%d, %lx, NULL)", i, (unsigned long) &act);
			}
		}
	}

	while(1){
		printf("sleeping for 5\n");
		sleep(5);
		if(sig_found){
			printf("sig_found: %d\n", sig_found);
			sig_found = 0;
		}
	}
}
