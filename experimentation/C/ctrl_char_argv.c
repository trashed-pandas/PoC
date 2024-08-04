
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv){
	int i;
	
	for(i = 0; i < argc; i++){
		memset(argv[i], 0, strlen(argv[i]));
	}

	//memcpy(argv[0], "Ni hao, shijie!\0", 16);
	memcpy(argv[0], "你好世界!\0", 16);

	sleep(20);

	return(0);
}
