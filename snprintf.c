#include <stdio.h>
#include <string.h>

int main(){

	char *string = "0123456789abcdef";
	int string_len;
	char string_storage[17];
	int string_storage_len;

	string_len = strlen(string);
	printf("string: %s\n", string);
	printf("string_len: %d\n", string_len);

	// snprintf() adds it's own '\0' byte. So string_len++.
	snprintf(string_storage, string_len, "%s", string);
	string_storage_len = strlen(string_storage);
	printf("string_storage: %s\n", string_storage);
	printf("string_storage_len: %d\n", string_storage_len);

	return(0);
}
