// Can't seem to get data to update, stick X and Y are always at 128

#include "ps4.h"

#define debug(sock, format, ...)\
	do {\
		char buffer[512];\
		int size = sprintf(buffer, format, ##__VA_ARGS__);\
		sceNetSend(sock, buffer, size, 0);\
	} while(0)

unsigned char data[512];

int _main(void) {
	// Init and resolve libraries
	initKernel();
	
	initLibc();
	initNetwork();
	
	
	// Get id of already loaded pad module
	int padModule;
	loadModule("libScePad.sprx", &padModule);
	
	// Unload it
	unloadModule(padModule);
	
	// Start fresh
	initPad();
	
	
	// Connect to debug server
	char socketName[] = "debug";
	
	struct sockaddr_in server;
	
	server.sin_len = sizeof(server);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = IP(192, 168, 0, 4);
	server.sin_port = sceNetHtons(9023);
	memset(server.sin_zero, 0, sizeof(server.sin_zero));
	
	int sock = sceNetSocket(socketName, AF_INET, SOCK_STREAM, 0);
	sceNetConnect(sock, (struct sockaddr *)&server, sizeof(server));
	
	
	// Browse /user/home/ to see all user IDs (in hex)
	int userID = 0x10000000;
	
	int pad = scePadOpen(userID, 0, 0, NULL);
	
	while(1) {
		scePadReadState(pad, data);
		
		debug(sock, "X: %d, Y: %d\n", data[4], data[5]);
		
		//sceNetSend(sock, data, 0x60, 0);
		
		sceKernelUsleep(1000000 / 60);
	}
	
	
	// Return to browser
	scePadClose(pad);
	sceNetSocketClose(sock);
	
	return 0;
}
