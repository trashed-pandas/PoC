

/**
* dhserver.cpp
* josuegomes.com
*/

#include <iostream>
#include <cstdlib>

#ifdef WIN32	
#include <windows.h>
#endif

#include <openssl/ssl.h>
#include <openssl/err.h>

using std::exit;

#define int_error(msg) handle_error(__FILE__, __LINE__, msg)

void handle_error(const char* file, int lineno, const char* msg)
{
	std::cerr << "\n** " << file << ":" << lineno << " " << msg << "\n";
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

	BIO *acc, *client;
	SSL* ssl;
	SSL_CTX* ctx;

	std::cout << "Creating context ... ";
	ctx = SSL_CTX_new(TLSv1_server_method());
	if (!ctx)
		int_error("SSL_CTX_new failed");
	std::cout << "done\n";

	DH* dh = DH_new();
	if (!dh)
		int_error("DH_new failed");

	std::cout << "Generating DH parameters ... ";
	if (!DH_generate_parameters_ex(dh, 2, DH_GENERATOR_2, 0))
		int_error("DH_generate_parameters_ex failed");
	std::cout << "done\n";

	std::cout << "Checking DH parameters ... ";
	int codes = 0;
	if (!DH_check(dh, &codes))
		int_error("DH_check failed");
	std::cout << "done\n";

	std::cout << "Generating DH keys ... ";
	if (!DH_generate_key(dh))
		int_error("DH_generate_key failed");
	std::cout << "done\n";

	std::cout << "Setting DH parameters ... ";
	SSL_CTX_set_tmp_dh(ctx, dh);	
	std::cout << "done\n";

	std::cout << "Setting cipher list ... ";
	if (SSL_CTX_set_cipher_list(ctx, "ADH-AES256-SHA") != 1)
		int_error("Error setting cipher list (no valid ciphers)");
	std::cout << "done\n";

	std::cout << "Creating server socket ... ";
	acc = BIO_new_accept("*:13011");
	if (!acc)
		int_error("Error creating server socket");
	std::cout << "done\n";

	std::cout << "Binding server socket ... ";
	if (BIO_do_accept(acc) <= 0)
		int_error("Error binding server socket");
	std::cout << "done\n";

	while (true) {
		std::cout << "Accepting connections ...\n";
		if (BIO_do_accept(acc) <= 0)
			int_error("Error accepting connection");

		client = BIO_pop(acc);

		std::cout << "Creating context ... ";
		if (!(ssl = SSL_new(ctx)))
			int_error("Error creating SSL context");
		std::cout << "done\n";

		SSL_set_bio(ssl, client, client);

		if (SSL_accept(ssl) <= 0)
			int_error("Error accepting SSL connection");

		std::cout << "SSL connection opened: " << SSL_get_cipher(ssl) << " " << 
			SSL_get_cipher_version(ssl) << " (" << SSL_get_cipher_bits(ssl, 0) << " bits)\n";

		char buff[256] = {0};

		int r = SSL_read(ssl, buff, sizeof buff);	
		if (r > 0) {
			std::cout << "Server received: <" << buff << ">\n";
			char answer[256] = {0};
			r = sprintf(answer, "I (the server) received this: <%s>", buff);
			SSL_write(ssl, answer, r);
		}

		SSL_shutdown(ssl);
		SSL_free(ssl);

		std::cout << "SSL connection finished\n";
	}

	SSL_CTX_free(ctx);
	BIO_free(acc);
	std::cout << "Server closed\n";
}
