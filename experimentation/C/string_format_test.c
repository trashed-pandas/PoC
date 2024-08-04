#include <stdio.h>
#include <string.h>

int main(){

	char buffer[256];
	char data[256];

	memset(buffer, 0, sizeof(buffer));

	fgets(buffer, sizeof(buffer), stdin);
	*(strchr(buffer, '\n')) = 0;

	printf("buffer: %s\n", buffer);

	sprintf(data, "%s", buffer);
	
	printf("data: %s\n", data);

	return(0);

}
