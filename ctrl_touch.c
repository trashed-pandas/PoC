
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv){
	int i;
	char out[256][2];

/*	
	for(i = 0; i < argc; i++){
		memset(argv[i], 0, strlen(argv[i]));
	}

	//memcpy(argv[0], "Ni hao, shijie!\0", 16);
	memcpy(argv[0], "你好世界!\0", 16);
*/

	
	for(i = 0; i < 256; i++){
		memset(out[i], 0, 2);
		out[i][0] = (char) i;
		printf("%d: %s\n", i, out[i]);
	}

	

	return(0);
}
