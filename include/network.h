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
	unsigned short sin_len;
	short sin_family;
	unsigned short sin_port;
	struct in_addr sin_addr;
	unsigned short sin_vport;
	char sin_zero[6];
};

struct sockaddr {
	unsigned short sin_len;
	unsigned short sa_family;
	char sa_data[14];
};

extern int (*sceNetSocket)(const char *, int, int, int);
extern int (*sceNetSocketClose)(int);
extern int (*sceNetConnect)(int, struct sockaddr *, int);
extern int (*sceNetSend)(int, const void *, size_t, int);
extern int (*sceNetBind)(int, struct sockaddr *, int);
extern int (*sceNetListen)(int, int);
extern int (*sceNetAccept)(int, struct sockaddr *, unsigned int *);
extern int (*sceNetRecv)(int, void *, size_t, int);

extern int (*sceNetGetsockname)(int, struct sockaddr *, unsigned int *);

extern const char (*sceNetInetNtop)(int af, const void *src, char *dst, int size);
extern int (*sceNetInetPton)(int af, const char *src, void *dst);

extern uint64_t (*sceNetHtonll)(uint64_t host64);
extern uint32_t (*sceNetHtonl)(uint32_t host32);
extern uint16_t (*sceNetHtons)(uint16_t host16);
extern uint64_t (*sceNetNtohll)(uint64_t net64);
extern uint32_t (*sceNetNtohl)(uint32_t net32);
extern uint16_t (*sceNetNtohs)(uint16_t net16);


void initNetwork(void);
