#pragma once

#include "types.h"

#define IP(a, b, c, d) (((a) << 0) + ((b) << 8) + ((c) << 16) + ((d) << 24))
#define htons(a) __builtin_bswap16(a)

#define AF_INET 0x0002

#define IN_ADDR_ANY 0

#define SOCK_STREAM 1
#define SOCK_DGRAM 2

struct in_addr {
	unsigned long s_addr;
};

struct sockaddr_in {
	short sin_family;
	unsigned short sin_port;
	struct in_addr sin_addr;
	char sin_zero[8];
};

extern int (*sceNetSocket)(const char *, int, int, int);
extern int (*sceNetSocketClose)(int);
extern int (*sceNetConnect)(int, struct sockaddr_in *, int);
extern int (*sceNetSend)(int, const void *, size_t, int);
extern int (*sceNetBind)(int, struct sockaddr_in *, int);
extern int (*sceNetListen)(int, int);
extern int (*sceNetAccept)(int, struct sockaddr_in *, int *);
extern int (*sceNetRecv)(int, void *, size_t, int);

void initNetwork(void);
