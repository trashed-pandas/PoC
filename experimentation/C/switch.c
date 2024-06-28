#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){

	int num;

	num = atoi(argv[1]);

	printf("hello\n");

	switch (num%2) {

		case 0:
			if(num == 2){
				printf("The number is 2!!!\n");
				break;
			}
			printf("%d: even!\n", num);
			break;

		case 1:
			printf("%d: odd!\n", num);
			break;

		default:
			printf("shouldn't be here!\n");
	}

	printf("good-bye\n");
	return(0);
}
