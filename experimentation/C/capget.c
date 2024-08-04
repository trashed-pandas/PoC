#include <stdio.h>
#include <sys/capability.h>

int main(){

	cap_t capability = cap_get_proc();

	int name_len;
	char *name = cap_to_text(capability, &name_len);

	if(name){
		printf("name: %s\n", name);
		printf("capability: %lx\n", (unsigned long) capability);
//		printf("*capability: %lx\n", (unsigned long) (*capability));
	}
}
