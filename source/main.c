#include "kernel.h"
#include "network.h"

struct in_addr ip = { IP(192, 168, 0, 4) };
const unsigned short port = 9023;
const char *message = "Hello!";

void _start(void) {
	int sock;
	
	struct sockaddr_in address = {
		AF_INET,
		port,
		ip,
	};
	
	initNetwork();
	
	sock = socket("test", AF_INET, SOCK_STREAM, 0);
	connect(sock, &address, sizeof(address));
	send(sock, message, sizeof(message), 0);
	close(sock);
	
	while(1) {
	}
}
