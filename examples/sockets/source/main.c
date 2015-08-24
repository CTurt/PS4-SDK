#include "kernel.h"

#include "libc.h"
#include "network.h"

#define debug(sock, format, ...)\
	do {\
		char buffer[512];\
		int size = sprintf(buffer, format, ##__VA_ARGS__);\
		sceNetSend(sock, buffer, size, 0);\
	} while(0)

int _main(void) {
	// Init and resolve libraries
	initLibc();
	initNetwork();


	// Connect to server and send message
	char socketName[] = "debug";

	struct sockaddr_in server;

	server.sin_family = sceNetHtons(AF_INET);
	server.sin_addr.s_addr = IP(192, 168, 0, 4);
	server.sin_port = sceNetHtons(9023);
	memset(server.sin_zero, 0, sizeof(server.sin_zero));

	int sock = sceNetSocket(socketName, AF_INET, SOCK_STREAM, 0);
	sceNetConnect(sock, (struct sockaddr *)&server, sizeof(server));

	debug(sock, "PID: %d", syscall(20));

	sceNetSocketClose(sock);


	// Return to browser
	return 0;
}
