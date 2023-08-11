#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv, char **envp){

	pid_t pid;


	pid = getpid();
	printf("pid: %d\n", (int) pid);
//	sleep(5);

	execve(argv[1], argv + 1, envp);


	printf("execve failed!\n");	

	return(0);
}
