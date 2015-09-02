#include "kernel.h"
#include "module.h"

#include "network.h"

int (*sceNetSocket)(const char *, int, int, int);
int (*sceNetSocketClose)(int);
int (*sceNetConnect)(int, struct sockaddr *, int);
int (*sceNetSend)(int, const void *, size_t, int);
int (*sceNetBind)(int, struct sockaddr *, int);
int (*sceNetListen)(int, int);
int (*sceNetAccept)(int, struct sockaddr *, unsigned int *);
int (*sceNetRecv)(int, void *, size_t, int);
int (*sceNetSocketAbort)(int , int );

int (*sceNetGetsockname)(int, struct sockaddr *, unsigned int *);
int (*sceNetGetsockopt)(int s, int level, int optname, void *restrict optval, socklen_t *restrict optlen);
int (*sceNetSetsockopt)(int s, int level, int optname, const void *optval, socklen_t optlen);

const char (*sceNetInetNtop)(int af, const void *src, char *dst, int size);
int (*sceNetInetPton)(int af, const char *src, void *dst);

uint64_t (*sceNetHtonll)(uint64_t host64);
uint32_t (*sceNetHtonl)(uint32_t host32);
uint16_t (*sceNetHtons)(uint16_t host16);
uint64_t (*sceNetNtohll)(uint64_t net64);
uint32_t (*sceNetNtohl)(uint32_t net32);
uint16_t (*sceNetNtohs)(uint16_t net16);

void initNetwork(void) {
	int libNet = sceKernelLoadStartModule("libSceNet.sprx", 0, NULL, 0, 0, 0);
	
	RESOLVE(libNet, sceNetSocket);
	RESOLVE(libNet, sceNetSocketClose);
	RESOLVE(libNet, sceNetConnect);
	RESOLVE(libNet, sceNetSend);
	RESOLVE(libNet, sceNetBind);
	RESOLVE(libNet, sceNetListen);
	RESOLVE(libNet, sceNetAccept);
	RESOLVE(libNet, sceNetRecv);
	RESOLVE(libNet, sceNetSocketAbort);
	

	RESOLVE(libNet, sceNetGetsockname);
	RESOLVE(libNet, sceNetGetsockopt);
	RESOLVE(libNet, sceNetSetsockopt);

	RESOLVE(libNet, sceNetInetNtop);
	RESOLVE(libNet, sceNetInetPton);

	RESOLVE(libNet, sceNetHtonll);
	RESOLVE(libNet, sceNetHtonl);
	RESOLVE(libNet, sceNetHtons);
	RESOLVE(libNet, sceNetNtohll);
	RESOLVE(libNet, sceNetNtohl);
	RESOLVE(libNet, sceNetNtohs);
}
