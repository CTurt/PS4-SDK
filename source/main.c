#include <string.h>

#include "kernel.h"
#include "network.h"

#define IP(a, b, c, d) (((a) << 24) + ((b) << 16) + ((c) << 8) + (d))

struct in_addr ip = { IP(192, 168, 0, 4) };
const unsigned short port = 9023;
const char *message = "Hello!";

int main(void) {
	int sock;
	
	struct sockaddr_in address = {
		AF_INET,
		port,
		ip,
	};
	
	initNetwork();
	
	sock = socket("test", AF_INET, SOCK_STREAM, 0);
	connect(sock, &address, sizeof(address));
	send(sock, message, strlen(message), 0);
	close(sock);
	
	while(1) {
	}
	
	return 0;
}
