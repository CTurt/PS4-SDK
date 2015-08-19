#include "network.h"

#include "module.h"

int (*sceNetSocket)(const char *, int, int, int);
int (*sceNetConnect)(int, struct sockaddr_in *, int);
int (*sceNetSend)(int, const void *, size_t, int);
int (*sceNetClose)(int);

void initNetwork(void) {
	RESOLVE(SCENET, sceNetSocket);
	RESOLVE(SCENET, sceNetConnect);
	RESOLVE(SCENET, sceNetSend);
	RESOLVE(SCENET, sceNetClose);
}
