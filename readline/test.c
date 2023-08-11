
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>

#include "./linenoise/linenoise.h"

int main(){

	char *command;	
	time_t t;
	pid_t child;

	if((child = fork())){

		while(1){
			time(&t);
			printf("%s", asctime(localtime(&t)));
			sleep(10);
		}

	}else{

		while((command = linenoise("revsh> "))){
			printf("DEBUG: command: %s\n", command);
			linenoiseHistoryAdd(command);
			linenoiseHistorySetMaxLen(50);
			linenoiseFree(command);
		}

	}

	return(0);
}
