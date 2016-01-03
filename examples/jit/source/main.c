#include "ps4.h"

int sock;

#define printf(format, ...)\
	do {\
		char buffer[512];\
		int size = sprintf(buffer, format, ##__VA_ARGS__);\
		sceNetSend(sock, buffer, size, 0);\
	} while(0)

int _main(void) {
	initKernel();
	
	initLibc();
	initNetwork();
	initJIT();
	
	
	struct sockaddr_in server;

	server.sin_len = sizeof(server);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = IP(192, 168, 0, 4);
	server.sin_port = sceNetHtons(9023);
	memset(server.sin_zero, 0, sizeof(server.sin_zero));

	sock = sceNetSocket("debug", AF_INET, SOCK_STREAM, 0);
	sceNetConnect(sock, (struct sockaddr *)&server, sizeof(server));
	
	
	void *executableAddress = NULL;
	void *writableAddress = NULL;
	void *dataAddress = NULL;
	
	allocateJIT(PAGE_SIZE, &executableAddress, &writableAddress);
	dataAddress = malloc(PAGE_SIZE);
	
	printf("Executable address: %p\n", executableAddress);
	printf("Writable address: %p\n", writableAddress);
	printf("Data address: %p\n", dataAddress);
	
	sceNetSocketClose(sock);
	
	
	// writableAddress and executableAddress are 2 different virtual mappings, with different protections, but which point to the same physical memory
	
	
	//unsigned char ret[] = { 0xc3 };
	unsigned char loop[] = { 0xeb, 0xfe };
	memcpy(writableAddress, loop, sizeof(loop));
	
	((void (*)())executableAddress)();
	
	
	return 0;
}
