#include <stdio.h>
#include <time.h>

int main(){

	time_t tloc;
	struct tm *tm;

	time(&tloc);
	tm = gmtime(&tloc);
	

	printf("time(): %s\n", asctime(tm));


	return(0);
}
