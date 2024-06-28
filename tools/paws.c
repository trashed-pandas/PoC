// clang -std=gnu99 -Wall -Wextra -pedantic    paws.c   -o paws

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


int main(void){

	int retval, i;
	struct sigaction act = {0};
	sigset_t all_sigs;

	act.sa_handler = signal_handler;

	sigfillset(&all_sigs);
	for(i = 1; i < NSIG; i++){
		if(sigismember(&all_sigs, i) && !((i == SIGKILL) || (i == SIGSTOP))){
			if((retval = sigaction(i, &act, NULL)) == -1){
				error(-1, errno, "sigaction(%d, %lx, NULL)", i, (unsigned long) &act);
			}
		}
	}

	printf("\nฅʕ·ᴥ·ʔฅ i hav paws!\n\n");
	fflush(stdout);

	int ctrl_c = 0;
	while(1){

		pause();
		printf("Signal: %d: %s\n", sig_found, strsignal(sig_found));

		if(sig_found == SIGINT){
			if(ctrl_c){
				printf("Exiting.\n");
				return(0);
			}
			printf("Press Ctrl+c again to exit.\n");
			ctrl_c = 1;

		}else{
			ctrl_c = 0;
		}

		fflush(stdout);
		sig_found = 0;
	}


	return(0);
}
