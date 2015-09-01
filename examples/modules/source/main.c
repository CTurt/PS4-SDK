#include "ps4.h"

int _main(void) {
	// Init and resolve libraries
	initKernel();
	
	initLibc();
	initNetwork();
	
	
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
	
	
	int module;
	
	// Load and initialise
	//module = sceKernelLoadStartModule("libScePad.sprx", 0, NULL, 0, 0, 0);
	
	// Just load for dumping
	loadModule("libScePad.sprx", &module);
	
	struct moduleInfo m;
	getModuleInfo(module, &m);
	
	sceNetSend(sock, m.codeBase, m.codeSize + m.dataSize, 0);
	
	
	// Return to browser
	sceNetSocketClose(sock);
	
	return 0;
}
