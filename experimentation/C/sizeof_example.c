#include <stdio.h>
#include <stdlib.h>

int main(){

	char foo[256];
	char *bar;

	bar = (char *) malloc(256);

	printf("sizeof(foo): %d\n", (int) sizeof(foo));
	printf("sizeof(bar): %d\n", (int) sizeof(bar));
	printf("sizeof(short): %d\n", (int) sizeof(short));
}
