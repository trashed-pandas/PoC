#include <stdio.h>
#include <sys/capability.h>

/*
	checks the capabilities set on this program, itself

	make sure to link to libcap
		$ LDFLAGS=-lcap make capget
		/usr/bin/clang -std=c18 -Wall -Wextra -pedantic  -lcap  capget.c   -o capget

	Run / demo as:
		./capget
		sudo setcap 'CAP_SYS_ADMIN=ep' capget
		./capget
		sudo setcap -r capget
		./capget
*/

int main(){

	cap_t capability = cap_get_proc();

	ssize_t name_len;
	char *name = cap_to_text(capability, &name_len);

	if(name_len){
		printf("name_len: %ld\n", name_len);
		printf("name: %s\n", name);
		printf("capability: %lx\n", (unsigned long) capability);
//		printf("*capability: %lx\n", (unsigned long) (*capability));
	}
}
