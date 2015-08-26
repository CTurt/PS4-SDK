// Can't seem to get data to change

#include "ps4.h"

unsigned char data[512];

int _main(void) {
	// Init and resolve libraries
	initLibc();
	initNetwork();
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
	
	
	// Browser /user/home/ to see all user IDs (in hex)
	int userID = 0x10000000;
	
	int pad = scePadOpen(userID, 0, 0, NULL);
	
	while(1) {
		scePadReadState(pad, data);
		sceNetSend(sock, data, 0x60, 0);
	}
	
	
	// Return to browser
	scePadClose(pad);
	sceNetSocketClose(sock);
	
	return 0;
}
