
#define _GNU_SOURCE

#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#define BUFFER_SIZE 256
#define SLEEP_DELAY 20

int main(int argc, char **argv){
	
	int sock_fd, retval;
	char out_buffer[BUFFER_SIZE];

	struct addrinfo *result, *rp;

	char *addr_index;
	

	if((argc != 2) || ((addr_index = index(argv[1], ':')) == NULL)){
		fprintf(stderr, "usage(): ...\n");
		exit(-1);
	}
	*addr_index = '\0';	
	addr_index++;

	if((retval = getaddrinfo(argv[1], addr_index, NULL, &result))){
		fprintf(stderr, "%s: getaddrinfor(): %s\n", program_invocation_name, gai_strerror(retval));
		exit(-1);
	}

	for(rp = result; rp != NULL; rp = rp->ai_next){

		if((retval = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1){
			continue;
		}
		sock_fd = retval;

		if((retval = connect(sock_fd, rp->ai_addr, rp->ai_addrlen)) != -1){
			break;
		}

		close(sock_fd);
	}

	if(rp == NULL){
		fprintf(stderr, "%s: Unable to connect to %s:%s. Quiting.", program_invocation_name, argv[1], addr_index);
		exit(-1);
	}
	freeaddrinfo(result);
	

	memset(out_buffer, 0, sizeof(out_buffer));
	snprintf(out_buffer, sizeof(out_buffer), "##\n");
	write(sock_fd, out_buffer, strlen(out_buffer));

	memset(out_buffer, 0, sizeof(out_buffer));
	snprintf(out_buffer, sizeof(out_buffer), "ppid: %d\n", (int) getppid());
	write(sock_fd, out_buffer, strlen(out_buffer));

	memset(out_buffer, 0, sizeof(out_buffer));
	snprintf(out_buffer, sizeof(out_buffer), "pid: %d\n", (int) getpid());
	write(sock_fd, out_buffer, strlen(out_buffer));

	memset(out_buffer, 0, sizeof(out_buffer));
	snprintf(out_buffer, sizeof(out_buffer), "pgid: %d\n", (int) getpgrp());
	write(sock_fd, out_buffer, strlen(out_buffer));

	memset(out_buffer, 0, sizeof(out_buffer));
	snprintf(out_buffer, sizeof(out_buffer), "sid: %d\n", (int) getsid(0));
	write(sock_fd, out_buffer, strlen(out_buffer));


	memset(out_buffer, 0, sizeof(out_buffer));
	snprintf(out_buffer, sizeof(out_buffer), "sleep(%d)\n", SLEEP_DELAY);
	write(sock_fd, out_buffer, strlen(out_buffer));
	sleep(20);
	
  memset(out_buffer, 0, sizeof(out_buffer));
  snprintf(out_buffer, sizeof(out_buffer), "close(0)\n");
  write(sock_fd, out_buffer, strlen(out_buffer));
	close(0);

  memset(out_buffer, 0, sizeof(out_buffer));
  snprintf(out_buffer, sizeof(out_buffer), "close(1)\n");
  write(sock_fd, out_buffer, strlen(out_buffer));
	close(1);

  memset(out_buffer, 0, sizeof(out_buffer));
  snprintf(out_buffer, sizeof(out_buffer), "close(2)\n");
  write(sock_fd, out_buffer, strlen(out_buffer));
	close(2);


	


	memset(out_buffer, 0, sizeof(out_buffer));
	snprintf(out_buffer, sizeof(out_buffer), "sleep(%d)\n", SLEEP_DELAY);
	write(sock_fd, out_buffer, strlen(out_buffer));
	sleep(20);
	
	memset(out_buffer, 0, sizeof(out_buffer));
	snprintf(out_buffer, sizeof(out_buffer), "return(0)\n");
	write(sock_fd, out_buffer, strlen(out_buffer));
	
	return(0);
}
