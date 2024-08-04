#include <stdio.h>

int main(int argc, char **argv){

	int i = 0;

  while(argv[i]){
    printf("argv[%d]: %s\n", i, argv[i]);

    i++;
  }

	printf("bar?? : %s\n", (*(argv + 2)));
	
	printf("foo?? : %s\n", 1[argv]);

	return(0);
}
