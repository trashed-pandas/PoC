#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 256

struct foo {
	char *bar;
	char *baz;

	void (*cleanup)(struct foo *active_pointer);
};

void clean_foo(struct foo *active_pointer);

int main(){

	struct foo this;
	struct foo *that;


	this.bar = calloc(BUFF_SIZE, sizeof(char));


	that = calloc(1, sizeof(struct foo));
	that->bar = calloc(BUFF_SIZE, sizeof(char));
	that->cleanup = &clean_foo;
	


	printf("this.bar: %lx\n", (unsigned long) (this.bar));
	printf("this.bar++: %lx\n", (unsigned long) (this.bar++));
	printf("(this.bar)++: %lx\n", (unsigned long) ((this.bar)++));
	printf("this.bar + 2: %lx\n", (unsigned long) (this.bar + 2));
	printf("(this.bar) + 2: %lx\n", (unsigned long) ((this.bar) + 2));

	that->cleanup(that);

	return(0);
}

void clean_foo(struct foo *active_pointer){

	free(active_pointer->bar);
	free(active_pointer->baz);
	free(active_pointer);
}
