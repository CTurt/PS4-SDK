#include "module.h"

#include "network.h"

int (*sceNetSocket)(const char *, int, int, int);
int (*sceNetSocketClose)(int);
int (*sceNetConnect)(int, struct sockaddr *, int);
int (*sceNetSend)(int, const void *, size_t, int);
int (*sceNetBind)(int, struct sockaddr *, int);
int (*sceNetListen)(int, int);
int (*sceNetAccept)(int, struct sockaddr *, int *);
int (*sceNetRecv)(int, void *, size_t, int);

void initNetwork(void) {
	int libNet;
	loadModule("libSceNet.sprx", &libNet);

	RESOLVE(libNet, sceNetSocket);
	RESOLVE(libNet, sceNetSocketClose);
	RESOLVE(libNet, sceNetConnect);
	RESOLVE(libNet, sceNetSend);
	RESOLVE(libNet, sceNetBind);
	RESOLVE(libNet, sceNetListen);
	RESOLVE(libNet, sceNetAccept);
	RESOLVE(libNet, sceNetRecv);
}
