#include "network.h"

#include "module.h"

int (*sceNetSocket)(const char *, int, int, int);
int (*sceNetConnect)(int, struct sockaddr_in *, int);
int (*sceNetSend)(int, const void *, size_t, int);
int (*sceNetSocketClose)(int);

void initNetwork(void) {
	int libNet;
	char libNetName[] = "libSceNet.sprx";
	loadModule(libNetName, &libNet);
	
	RESOLVE(libNet, sceNetSocket);
	RESOLVE(libNet, sceNetConnect);
	RESOLVE(libNet, sceNetSend);
	RESOLVE(libNet, sceNetSocketClose);
}
