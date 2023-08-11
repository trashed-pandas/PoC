
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NANOS_PER_SEC 999999999

int main(){

	struct timespec req;
	long nanos, secs, old_secs;

	// initial values
	nanos = 0;
	secs = 0;
	old_secs = 0;
	memset(&req, 0, sizeof(req));	

	// loop until we exit() or get killed
	while(1){

		printf("secs: %ld, nanos: %ld\n", secs, nanos);
	
		req.tv_sec = secs;
		req.tv_nsec = nanos;

		// sleep
		nanosleep(&req, NULL);

		// rest of the loop is setting up for the next round.
		// use an exponential backing-off algorithm

		if(!nanos){
			nanos = 1;
		}else{
			nanos = nanos << 1;
		} 

		if(nanos > NANOS_PER_SEC){
			nanos = 0;

			if(!secs){
				secs = 1;
			}else{
				old_secs = secs;
				secs = secs << 1;
			}

			if(secs < old_secs){
				exit(0);
			}
		}
	}

	return(0);
}
