
#define _GNU_SOURCE

#include <errno.h>
#include <error.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

volatile sig_atomic_t sig_found = 0;


void sig_handler(int signal);


void signal_handler(int signal){
	sig_found = signal;
}


int main(){
	struct sigaction act;
	time_t tloc;
	struct tm *tm;

	memset(&act, 0, sizeof(act));
	act.sa_handler = signal_handler;
	act.sa_flags = SA_RESTART;

	sigaction(SIGINT, &act, NULL);

	while(1){
		time(&tloc);
		tm = gmtime(&tloc);
		printf("\npause(): %s\n", asctime(tm));
		pause();
	}
}
