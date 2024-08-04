
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MY_STRING "foo bar baz!"

void goodbye();

int my_global = 10;

int main(){

	printf("Hello World!\n");
//	goodbye();

	return(0);
}

void goodbye(){

	char *greeting;
	
	greeting = (char *) malloc(sizeof(MY_STRING) + 1);
	memset(greeting, 0, sizeof(MY_STRING) + 1);
	memcpy(greeting, MY_STRING, sizeof(MY_STRING));

	printf("my_global: %d\n", my_global);

	printf("greeting: %s\n", greeting); 
	sleep(20);

	printf("Good-bye World!\n");

	
}
