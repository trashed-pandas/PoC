#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <values.h>

void int_to_binary_ascii(int input);

int main(int argc, char **argv){
	
	int input = strtol(argv[1], NULL, 10);

	int duration = 0;	
	if(argc == 3){
		duration = strtol(argv[2], NULL, 10);
	}

	int mod = 0;
//	int current = input;
	int current = 1;
	int last = current;
	while(!(current < last)){

		mod = current % input;
	
		if(!mod){
			printf("\x1b[31m");
		}
		int_to_binary_ascii(current);
		if(!mod){
			printf("\x1b[0m");
		}

		sleep(duration);

		last = current;
//		current += input;
		current++;
	}

	return(0);
}

void int_to_binary_ascii(int input){

	unsigned int bitwise = 1;
	bitwise <<= ((sizeof(int) * BITSPERBYTE) - 1);

	while(bitwise){
		if(input & bitwise){
			printf("1");
		}else{
			printf("0");
		}
		bitwise >>= 1;
	}
	printf("\n");
}
