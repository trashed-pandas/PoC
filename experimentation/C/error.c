#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <string.h>

int main(){
	int i;
	
	printf("Hello\n");
	for(i = 0; i < 133; i++){
//		error(0, i, "error(%d, %d, XXX)", 0, i);
		printf("%d: %s\n", i, strerror(i));
	}
	printf("world\n");
}
