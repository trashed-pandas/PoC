#include <stdio.h>
#include <unistd.h>

int GLOBAL_BAR;
int GLOBAL_BAZ = 23;

int main(){
	int i;
	char *foo = "Hello world!";
	char *bar;
	char baz[32];
	
	printf("&foo: %p\n", (void *) &foo);
	printf("foo: %s\n", foo);
	printf("&bar: %#lx\n", (unsigned long) &bar);
	printf("bar: %lx\n", (unsigned long) bar);
	printf("baz: %lx\n", (unsigned long) baz);
	printf("*baz: ");
	for(i = 0; i < 32; i++){
		printf("%x ", (unsigned int) baz[i]);
	}
	printf("\n");
	printf("&GLOBAL_BAR: %lx\n", (unsigned long) &GLOBAL_BAR);
	printf("GLOBAL_BAR: %d\n", GLOBAL_BAR);
	printf("&GLOBAL_BAZ: %lx\n", (unsigned long) &GLOBAL_BAZ);
	printf("GLOBAL_BAZ: %d\n", GLOBAL_BAZ);

	sleep(20);
	
}
