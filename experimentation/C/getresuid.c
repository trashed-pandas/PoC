#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>


int main(){
	uid_t ruid, euid, suid;

	getresuid(&ruid, &euid, &suid);
	setresuid(euid, euid, euid);
	getresuid(&ruid, &euid, &suid);

	printf("ruid: %d\n", ruid);
	printf("euid: %d\n", euid);
	printf("suid: %d\n", suid);

	return(0);
}
