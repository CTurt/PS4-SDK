#include <stdlib.h>

#include "kernel.h"
#include "network.h"

int _main(void) {
	// Load modules
	int libc;
	char libcName[] = "libSceLibcInternal.sprx";
	loadModule(libcName, &libc);
	
	int libNet;
	char libNetName[] = "libSceNet.sprx";
	loadModule(libNetName, &libNet);
	
	
	// Resolve functions
	void *(*memset)(void *destination, int value, size_t num);
	RESOLVE(libc, memset);
	
	void *(*memcpy)(void *destination, const void *source, size_t num);
	RESOLVE(libc, memcpy);
	
	char *(*strcpy)(char *destination, const char *source);
	RESOLVE(libc, strcpy);
	
	int (*sceNetSocket)(const char *, int, int, int);
	RESOLVE(libNet, sceNetSocket);
	
	int (*sceNetConnect)(int, struct sockaddr_in *, int);
	RESOLVE(libNet, sceNetConnect);
	
	int (*sceNetSend)(int, const void *, size_t, int);
	RESOLVE(libNet, sceNetSend);
	
	int (*sceNetSocketClose)(int);
	RESOLVE(libNet, sceNetSocketClose);
	
	
	// Connect to server and send message
	char socketName[] = "debug";
	char message[] = "Hello!";
	
	struct sockaddr_in server;
	
	server.sin_family = htons(AF_INET);
	server.sin_addr.s_addr = IP(192, 168, 0, 4);
	server.sin_port = htons(9023);
	memset(server.sin_zero, 0, sizeof(server.sin_zero));
	
	int sock = sceNetSocket(socketName, AF_INET, SOCK_STREAM, 0);
	sceNetConnect(sock, &server, sizeof(server));
	sceNetSend(sock, message, sizeof(message), 0);
	sceNetSocketClose(sock);
	
	
	// Return to browser
	return 0;
}
