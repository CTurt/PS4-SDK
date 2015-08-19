#pragma once

#include <stdlib.h>

#define IP(a, b, c, d) (((a) << 24) + ((b) << 16) + ((c) << 8) + (d))

#define SCENET 0xe

#define AF_INET 2
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
extern int (*sceNetConnect)(int, struct sockaddr_in *, int);
extern int (*sceNetSend)(int, const void *, size_t, int);
extern int (*sceNetClose)(int);

void initNetwork(void);
