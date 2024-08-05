#include <stdio.h>
#include <string.h>
#include <unistd.h>

// demonstrates that argv is malleable. It is your stack after all.
int main(int argc, char **argv){
	char *string = "totally foobar";
	memcpy(argv[0], string, strlen(string));
	pause();
}
