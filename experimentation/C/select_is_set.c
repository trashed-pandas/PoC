
/*
	Beej's code, slightly modified by em.
	http://beej.us/guide/bgnet/output/html/multipage/selectman.html
*/

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(){
//	int s1, s2, n;
	int s1, n;
	fd_set readfds, writefds;
	struct timeval tv;
//	char buf1[256], buf2[256];

	// pretend we've connected both to a server at this point
	//s1 = socket(...);
	//s2 = socket(...);
	//connect(s1, ...)...
	//connect(s2, ...)...

	s1 = STDIN_FILENO;
	//s2 = STDOUT_FILENO;

	// clear the set ahead of time
	FD_ZERO(&readfds);

	// add our descriptors to the set
	FD_SET(s1, &readfds);
	FD_SET(s1, &writefds);

	// since we got s2 second, it's the "greater", so we use that for
	// the n param in select()
	n = s1 + 1;

	// wait until either socket has data ready to be recv()d (timeout 10.5 secs)
	tv.tv_sec = 10;
	tv.tv_usec = 500000;
	int rv = select(n, &readfds, &writefds, NULL, &tv);

	int retval;
	if (rv == -1) {
		perror("select"); // error occurred in select()
	} else if (rv == 0) {
		printf("Timeout occurred!  No data after 10.5 seconds.\n");
	} else {
		if (FD_ISSET(s1, &readfds)) {
			printf("Data to read!\n");
//			recv(s1, buf1, sizeof buf1, 0);
		}
		// one or both of the descriptors have data
		if (FD_ISSET(s1, &writefds)) {
			printf("Data to write!\n");
//			recv(s2, buf2, sizeof buf2, 0);
		}
	}

	return(0);
}
