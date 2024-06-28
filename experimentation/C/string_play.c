#include <stdio.h>

#define HELLO "hello"
#define WORLD "world"

int main(){

	char *ptr = HELLO "/" WORLD;
	printf("ptr: %lx: %s\n", (unsigned long) ptr, ptr);

	char array[] = WORLD "/" HELLO;
	printf("array: %lx: %s\n", (unsigned long) array, array);

	return(0);
}
