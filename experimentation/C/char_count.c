
#include <limits.h>
#include <error.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/*
	First, I have no memory of this or what it was originally intended to do.
	It looked kinda like a naive implementation of strcmp() but didn't actually work. 
	So I cleaned it up and now it works. Feed one of the strings to compare  on the command line and the other through stdin.
	
	empty@monkey:.../C$ echo foo | ./char_count foo
	count: 3
	
	empty@monkey:.../C$ echo foo | ./char_count foobar
	count: 3
	
	empty@monkey:.../C$ echo foo | ./char_count xfoobar
	count: 0
*/

int main(int argc, char **argv){
	char current_char;
	int count = 0;

	if(argc != 2){
		error(-1, 0, "usage(): ...\n");
	}

	int argv_1_len = strlen(argv[1]);	
	int argv_1_index = 0;
	while(read(0, &current_char, 1)){

/*
		printf("count: %d\n", count);
		printf("current_char: %c\n", current_char);
		printf("argv[1][%d]: %c\n", argv_1_index, argv[1][argv_1_index]);
*/

		if(current_char != argv[1][argv_1_index])
			break;

		count++;
		argv_1_index++;

		if(argv_1_index == argv_1_len)
			break;

		if(count == INT_MAX){
			error(-1, 0, "OVERFLOW!!!");
		}
	}

	printf("count: %d\n", count);

	return(0);
}
