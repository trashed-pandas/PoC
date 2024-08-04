#include <stdio.h>

int main(int argc, char **argv){

	char **tmp_vector;
	char *tmp_string;

	tmp_vector = argv;
	tmp_string = *(tmp_vector++); 

	while(tmp_string){
		printf("tmp_string -> %s\n", tmp_string);
		tmp_string = *(tmp_vector++);
	}

	return(0);
}
