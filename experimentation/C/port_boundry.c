#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>

int main(){

	struct addrinfo hints, *res;
	int sockfd, retval;

	char *port;


	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;


	port = "1023";
	if((retval = getaddrinfo(NULL, port, &hints, &res)) != 0){
		error(-1, retval, "main(): getaddrinfo()");
	}

	if((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1){
		error(-1, errno, "main(): socket()");
	}

	if((retval = bind(sockfd, res->ai_addr, res->ai_addrlen)) == -1){
		error(-1, errno, "main(): bind()");
	}

	printf("%s sockfd: %d\n", port, sockfd);
	close(sockfd);

	port = "1024";
	if((retval = getaddrinfo(NULL, "1023", &hints, &res)) != 0){
		error(-1, retval, "main(): getaddrinfo()");
	}

	if((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1){
		error(-1, errno, "main(): socket()");
	}

	if((retval = bind(sockfd, res->ai_addr, res->ai_addrlen)) == -1){
		error(-1, errno, "main(): bind()");
	}

	printf("%s sockfd: %d\n", port, sockfd);
	close(sockfd);

	return(0);
}
