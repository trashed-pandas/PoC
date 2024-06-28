#include <stdio.h>
#include <wordexp.h>
#include <stdlib.h>

int main(int argc, char **argv){

	wordexp_t p;
	char **w;
	int i, j;

	j = 1;
	while(argv[j]){

		printf("DEBUG: %s\n", argv[j]);

		wordexp(argv[j], &p, 0);

		printf("DEBUG: p.we_wordc: %d\n", p.we_wordc);
		printf("DEBUG: p.we_wordv: %lx\n", (unsigned long) p.we_wordv);

		w = p.we_wordv;
		for (i = 0; i < p.we_wordc; i++)
			printf("%s\n", w[i]);
		wordfree(&p);

		j++;
	}

	return(0);
}

