#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv){
//	argv[0] = "totally foobar";
	char *string = "totally foobar";
	memcpy(argv[0], string, strlen(string));
	pause();
}
