#include <stdio.h>

int main(){

	char *foo, *bar = NULL;
	char *baz = NULL, *bat = NULL;
	
	printf("foo: %ld\n", (unsigned long) foo);
	printf("bar: %ld\n", (unsigned long) bar);
	printf("baz: %ld\n", (unsigned long) baz);
	printf("bat: %ld\n", (unsigned long) bat);

	return 0;
}
