#include "kernel.h"
#include "network.h"

int _main(void) {
	// Pass address of a syscall gadget in rcx
	register f rcx asm("rcx");
	directSyscall = rcx;
	
	
	// Load modules
	int libc;
	char libcName[] = "libSceLibcInternal.sprx";
	loadModule(libcName, &libc);
	
	int libNet;
	char libNetName[] = "libSceNet.sprx";
	loadModule(libNetName, &libNet);
	
	
	// Resolve functions
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
	
	int (*sceNetClose)(int);
	RESOLVE(libNet, sceNetClose);
	
	
	// Connect to server
	char socketName[] = "debug";
	struct sockaddr_in server;
	
	server.sin_family = htons(AF_INET);
	server.sin_addr.s_addr = IP(192, 168, 0, 4);
	server.sin_port = htons(9023);
	server.sin_zero[0] = 0;
	server.sin_zero[1] = 0;
	server.sin_zero[2] = 0;
	server.sin_zero[3] = 0;
	server.sin_zero[4] = 0;
	server.sin_zero[5] = 0;
	server.sin_zero[6] = 0;
	server.sin_zero[7] = 0;
	
	
	// Send a message
	char message[] = "Hello!";
	
	int sock = sceNetSocket(socketName, AF_INET, SOCK_STREAM, 0);
	sceNetConnect(sock, &server, sizeof(server));
	sceNetSend(sock, message, sizeof(message), 0);
	sceNetClose(sock);
	
	
	// Return to browser
	return 0;
}
