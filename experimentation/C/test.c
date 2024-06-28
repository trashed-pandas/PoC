#include <stdio.h>

#define BUFFER_SIZE 32

struct chunk_o_mem {
  unsigned long address;
  int count;
  char contents[BUFFER_SIZE];
};

void foo(struct chunk_o_mem *bar);

int main(){
	struct chunk_o_mem bar;
	void *baz;

	printf("DEBUG: main()\n");	
	printf("sizeof(bar): %d\n", (int) sizeof(bar));
	printf("sizeof(baz): %d\n", (int) sizeof(baz));

	foo(&bar);
	return(0);
}

void foo(struct chunk_o_mem *bar){
	printf("DEBUG: foo()\n");	
	printf("sizeof(bar): %d\n", (int) sizeof(bar));
	printf("sizeof(*bar): %d\n", (int) sizeof(*bar));
}
