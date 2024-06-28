
#include "char_count.h"



int main(int argc, char **argv){
	char now;
	int count = 0;

	if(argc != 2){
		error(-1, 0, "usage(): ...\n");
	}
	
	while(read(0, &now, 1)){
		if(now == argv[1][0]){

			my_increment(&count);			
	
			if(count == INT_MAX){
				error(-1, 0, "OVERFLOW!!!");
			}
		}
	}

	printf("count: %d\n", count);
	
	return(0);
}

void my_increment(int *count){
	count++;
}
