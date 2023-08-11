
// 005a 0000 0000 0000
#define SOCKS_V4_REPLY "\x00\x5a\x00\x00\x00\x00\x00\x00"
#define SOCKS_V4_REPLY_LEN 8

// 0500
#define SOCKS_V5_AUTH_REPLY "\x05\x00"
#define SOCKS_V5_AUTH_REPLY_LEN 2

// 0500 0001 0000 0000 0000
#define SOCKS_V5_REPLY "\x05\x00\x00\x01\x00\x00\x00\x00\x00\x00"
#define SOCKS_V5_REPLY_LEN 10

#include <stdio.h>
#include <unistd.h>

int main(){

	char *buff_ptr;
	int buff_ptr_len;

//	buff_ptr = SOCKS_V4_REPLY;
//	buff_ptr_len = SOCKS_V4_REPLY_LEN;
//	buff_ptr = SOCKS_V5_AUTH_REPLY;
//	buff_ptr_len = SOCKS_V5_AUTH_REPLY_LEN;
	buff_ptr = SOCKS_V5_REPLY;
	buff_ptr_len = SOCKS_V5_REPLY_LEN;

	write(1, buff_ptr, buff_ptr_len);

	return(0);
}
