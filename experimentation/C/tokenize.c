#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int count_tokens(const char *s);


// Pull the tokens out of a string.
// This example tokenizes a string of numbers that are ':' delimited.
// e.g.  123:456:789
int main(int argc, char **argv) {

	// Only the program name and a single string for tokenizing should be present
	if (argc != 2){
		fprintf(stderr, "Usage:...\n");
		return 1;
	}

	// lexically analyze and count the tokens
	int count;
	if ((count = count_tokens(argv[1])) == -1){
		// Error already printed by the callee
		return 1;
	}
		
	// grab memory for token storage
	long *tokens = calloc(count, sizeof(long));

	if (!tokens) {
		fprintf(stderr, "Error: calloc()\n");
		return 1;
	}

	// extract and store the tokens
	// note: strtol() will do the tokenizing for us
	char *s = argv[1];
	long *l = tokens;
	do {
		errno = 0;
		*l++ = strtol(s, &s, 10);

		if (errno) {
			fprintf(stderr, "Error: %s\n", strerror(errno));
			return 1;
		}

	} while (*s++);

	// Now step through a print the tokens for the demo
	for (int i = 0; i < count; i++){
		printf("%ld\n", tokens[i]);
	}

	return 0;
} 


// lexically analize the string and count the number of embedded tokens
int count_tokens(const char *s){

	// Case: The input string is the empty string.
	if (s[0] == '\0') {
		fprintf(stderr, "Error: Empty input!\n");
		return -1;
	} 

	// Loop over the string, one character at a time.
	int count = 0;
	const char *p = s;
	for (; p[0]; p++){

		// Case: String has a char other than [0-9:]
		if (!(isdigit((unsigned char) p[0]) || p[0] == ':')){
			fprintf(stderr, "Error: Invalid char!\n");
			return -1;
		}

		// Case: Delimiter found
		if (p[0] == ':'){

			// Cases: String starts with a delimiter, or two delimiters in a row.
			if (p == s || p[-1] == ':'){
				fprintf(stderr, "Error: Invalid format!\n");
				return -1;
			}
			count++;
		}
	}	

	// Case: String ends with a delimiter
	if (p[-1] == ':'){
		fprintf(stderr, "Error: Trailing ':'!\n");
		return -1;
	}

	return count + 1;
}
