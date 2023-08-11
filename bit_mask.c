#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
	int number;
	char bit_array[(UCHAR_MAX + 1) / CHAR_BIT], *strtol_check=NULL;

	// First, demonstrate storage.
	memset(bit_array, 0, sizeof(bit_array));
	while(--argc){
		number = strtol(argv[argc], &strtol_check, 10);	
		if(errno){
			fprintf(stderr, "%s is causing problems: %s. Skipping.\n", argv[argc], strerror(errno));
			continue;
		}
		if(strtol_check == argv[argc]){
			fprintf(stderr, "%s is not a number! Skipping.\n", argv[argc]);
			continue;
		}

		if((number > UCHAR_MAX) || (number < 0)){
			fprintf(stderr, "%s is outside of the range 0 - %d. Skipping.\n", argv[argc], UCHAR_MAX);
			continue;
		}

		printf("saving: %d\n", number);
		bit_array[number / CHAR_BIT] |= (1 << (number % CHAR_BIT));

		if(argc == 1){
			printf("\n");
		}
	}	
	
	// Now, demonstrate retrieval.
	for(number = 0; number <= UCHAR_MAX; number++){
		if(bit_array[number / CHAR_BIT] & (1 << (number % CHAR_BIT))){
			printf("found: %d\n", number);
		}
	}

	return(0);
}
