#include <stdio.h>
#include <time.h>

int main(){

	time_t time_struct;
	char *ctime_string;
	char *gmtime_string;

	time_struct = time(NULL);
	gmtime_string = asctime(gmtime(&time_struct));
	ctime_string = ctime(&time_struct);

	printf("ctime_string: %s", ctime_string);
	printf("gmtime_string: %s", gmtime_string);

	return(0);
}
