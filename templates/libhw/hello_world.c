#include <stdio.h>
#include "libhw.h"


int main(void){
	
	printf("\n");	
	printf("-- hello() called --\n");	
	hello();
	printf("-- hello() returned --\n");	

	printf("\n");	
	printf("-- goodbye() called --\n");	
	goodbye();
	printf("-- goodbye() returned --\n");	

	return(0);
}
