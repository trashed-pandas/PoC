

#include "paws.h"
#include "sigmap.h"


volatile sig_atomic_t sig_found = 0;


void signal_handler(int signal);
void dump_rusage(void);
void dump_sig_count(int *sig_count);



/***************************************************************************************************************************************************************
 *
 * int main(void);
 *
 *  Inputs: None. No arguments are processed. No env variables mangled.
 *  Outputs: main() returns 0 to indicate success and maintain POSIX compliance. It does not handle other return codes. It uses error() to handle errors which
 *           itself handles the exit() and ultimately the process return code in the nonzero case.
 *  Purpose: Prepare the process to be manually inspected by a human, and to assist in that inspection.
 *  Strategy: 
 *  	- Setup the signal handling to catch all catchable signals.
 *  	- Be extra kawaii!!
 *  	- Report useful process information.
 *  	- Start an infinite loop.
 *  	- pause() to allow the human to inspect the process.
 *  	- Report signal information for the signal received
 *  	- Handle special signals
 *  	- SIGINT is special. Receiving it twice in a row is the exit mechanism.
 *
 ***************************************************************************************************************************************************************/
int main(void){

	int retval, i;

	// Setup the signal handling to catch all catchable signals.
	struct sigaction act = {0};
	sigset_t all_sigs;
	act.sa_handler = signal_handler;

	sigfillset(&all_sigs);
	for(i = 1; i < _NSIG; i++){
		if(sigismember(&all_sigs, i) && !((i == SIGKILL) || (i == SIGSTOP))){
			if((retval = sigaction(i, &act, NULL)) == -1){
				error(-1, errno, "sigaction(%d, %lx, NULL)", i, (unsigned long) &act);
			}
		}
	}

	// Be extra kawaii!!
	printf("\nKawaii Bear says:\n");
	printf("\ti hav paws!\n\t");
	printf("ฅʕ·ᴥ·ʔฅ");
	printf("\n\n");

	// Report useful process information.
	printf("pid: %d\nppid: %d\n", getpid(), getppid());
	printf("uid: %d\neuid: %d\ngid: %d\n\n", getuid(), geteuid(), getgid());
	fflush(stdout);

	// Start an infinite loop.
	int ctrl_c = 0;
	int sig_count[_NSIG] = {0};
	const struct sigmap_element *sigmap_element_found;
	while(1){

		// pause() to allow the human to inspect the process.
		pause();

		if(sig_found <= _NSIG){
			sig_count[sig_found]++;
		}else{
			error(-1, ENOSYS, "sig_found: %d\n", sig_found);
		}

		// Report signal information for the signal received
		sigmap_element_found = find_sigmap_element(sig_found);
		printf("\nSignal Number: %d\n", sig_found);
		if(sigmap_element_found->number){
			printf("Signal Name: %s\n", sigmap_element_found->name);
			printf("Description: %s\n", strsignal(sigmap_element_found->number));
		}else{
			printf("Description: Unknown Signal\n");
		}

		// Handle special signals
		if(sig_found == SIGUSR1){
			printf("SIGUSR1 found. Printing resource usage statistics:\n");
			dump_rusage();

		}else if(sig_found == SIGUSR2){
			printf("SIGUSR2 found. Printing counts of signals received:\n");
			dump_sig_count(sig_count);

		}else if(sig_found == SIGINT){
			// SIGINT is special. Receiving it twice in a row is the exit mechanism.
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


/***************************************************************************************************************************************************************
 *
 * void signal_handler(int signal);
 *
 *  Inputs: The signal number that invoked it.
 *  Outputs: None.
 *  Purpose: Our custom signal handler, invoked by the kernel upon signal delivery.
 *  Strategy: 
 *  	- Record the signal number of the signal received.
 *
 ***************************************************************************************************************************************************************/
void signal_handler(int signal){

	// Record the signal number of the signal received.
	sig_found = signal;
}


/***************************************************************************************************************************************************************
 *
 * void dump_rusage(void);
 *
 *  Inputs: None.
 *  Outputs: None.
 *  Purpose: Report resource usage statistics.
 *  Strategy: 
 *  	- Query rusage
 *  	- Print rusage
 *
 ***************************************************************************************************************************************************************/
void dump_rusage(void){
	struct rusage usage;
	errno = 0;

	// Query rusage
	if(getrusage(RUSAGE_SELF, &usage)){
		fprintf(stderr, "%s\n", strerror(errno));
		error(-1, errno, "getrusage(RUSAGE_SELF, %lx)", (unsigned long) &usage);
	}

	// Print rusage
	printf("\tusage.ru_utime: user CPU time used:\t%ld.%ld\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
	printf("\tusage.ru_stime: system CPU time used:\t%ld.%ld\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
	printf("\tusage.ru_maxrss: maximum resident set size:\t%ld\n", usage.ru_maxrss);
	printf("\tusage.ru_ixrss: integral shared memory size:\t%ld\n", usage.ru_ixrss);
	printf("\tusage.ru_idrss: integral unshared data size:\t%ld\n", usage.ru_idrss);
	printf("\tusage.ru_isrss: integral unshared stack size:\t%ld\n", usage.ru_isrss);
	printf("\tusage.ru_minflt: page reclaims (soft page faults):\t%ld\n", usage.ru_minflt);
	printf("\tusage.ru_majflt: page faults (hard page faults):\t%ld\n", usage.ru_majflt);
	printf("\tusage.ru_nswap: swaps:\t%ld\n", usage.ru_nswap);
	printf("\tusage.ru_inblock: block input operations:\t%ld\n", usage.ru_inblock);
	printf("\tusage.ru_oublock: block output operations:\t%ld\n", usage.ru_oublock);
	printf("\tusage.ru_msgsnd: IPC messages sent:\t%ld\n", usage.ru_msgsnd);
	printf("\tusage.ru_msgrcv: IPC messages received:\t%ld\n", usage.ru_msgrcv);
	printf("\tusage.ru_nsignals: signals received:\t%ld\n", usage.ru_nsignals);
	printf("\tusage.ru_nvcsw: voluntary context switches:\t%ld\n", usage.ru_nvcsw);
	printf("\tusage.ru_nivcsw: involuntary context switches:\t%ld\n", usage.ru_nivcsw);
	printf("\n");
}


/***************************************************************************************************************************************************************
 *
 * void dump_sig_count(int *sig_count);
 *
 *  Inputs: A pointer to the array that tracks the counts of the signals received.
 *  Outputs: None.
 *  Purpose: Print the non-zero counts of signals received.
 *  Strategy: 
 *  	- Loop through all signals
 *  	- Print the count for the matching signal
 *
 ***************************************************************************************************************************************************************/
void dump_sig_count(int *sig_count){

	const struct sigmap_element *sigmap_element_found;

	printf("Counts of signals received:\n");

	// Loop through all signals
	for(int i = 1; i <= _NSIG ; i++){
		if(sig_count[i]){
			sigmap_element_found = find_sigmap_element(i);

			// Print the count for the matching signal
			printf("\t%7s:\t%d\n", sigmap_element_found->name, sig_count[i]);
		}
	}
	printf("\n");
}
