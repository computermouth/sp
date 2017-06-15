
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

int main ( void ) {
	
	char *hostname = "www.google.com";
	
	int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    server = gethostbyname(hostname);
	
	
	return 0;
}
