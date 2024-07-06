/* clang -ansi -Wall -Wextra -pedantic -o hw hw.c */

#include <signal.h>
#include <stdio.h>

int main(void){

	printf("_NSIG: %d\n", _NSIG);

	return 0;
}
