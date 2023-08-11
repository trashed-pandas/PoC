

/*
* dhclient.cpp

* same as:
*  openssl s_client -tls1 -cipher ADH-AES256-SHA -connect localhost:13011
*
* josuegomes.com
*/

#include <iostream>
#include <cstdlib>
#include <string>

#ifdef WIN32
#include <windows.h>
#endif

#include <openssl/ssl.h>
#include <openssl/err.h>

using std::exit;

#define int_error(msg) handle_error(__FILE__, __LINE__, msg)

void handle_error(const char* file, int lineno, const char* msg)
{
	std::cerr << "** " << file << ":" << lineno << " " << msg << "\n";
	ERR_print_errors_fp(stderr);
	exit(-1);
}


int main()
{

	std::cout << "Initializng OpenSSL library ... ";
	if (!SSL_library_init())
		int_error("OpenSSL initialization failed");
	std::cout << "done\n";
	SSL_load_error_strings();
	BIO* conn;
	SSL* ssl;
	SSL_CTX* ctx;

	std::cout << "Creating context ... ";
	ctx = SSL_CTX_new(TLSv1_client_method());
	if (!ctx)
		int_error("SSL_CTX_new failed");
	std::cout << "done\n";

	std::cout << "Setting cipher list ... ";
	if (SSL_CTX_set_cipher_list(ctx, "ADH-AES256-SHA") != 1)
		int_error("Error setting cipher list (no valid ciphers)");
	std::cout << "done\n";
	std::cout << "Creating connection ... ";
	conn = BIO_new_connect("localhost:13011");
	if (!conn)
		int_error("Error creating connection");
	std::cout << "done\n";

	std::cout << "Connecting to server ... ";
	if (BIO_do_connect(conn) <= 0)
		int_error("Error connecting to server");
	std::cout << "done\n";

	std::cout << "Creating context ... ";
	if (!(ssl = SSL_new(ctx)))
		int_error("Error creating an SSL context");
	std::cout << "done\n";

	SSL_set_bio(ssl, conn, conn);

	std::cout << "Opening connection ... ";
	if (SSL_connect(ssl) <= 0)
		int_error("Error connecting SSL object");
	std::cout << "done\n";

	SSL_write(ssl, "Lorem ipsum dolor sit amet", 26);

	char buff[64] = {0};
	int bread = SSL_read(ssl, buff, sizeof buff);
	if (bread > 0)
		std::cout << "Server answered with: <" << std::string(buff, bread) << ">\n";

	SSL_shutdown(ssl);
	SSL_free(ssl);

	std::cout << "SSL connection finished\n";

	SSL_CTX_free(ctx);
	std::cout << "Client finished\n";
}


