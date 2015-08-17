#pragma once

#include <stdlib.h>

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

extern int (*socket)(const char *, int, int, int);
extern int (*connect)(int, struct sockaddr_in *, int);
extern int (*send)(int, const void *, size_t, int);
extern int (*close)(int);

void initNetwork(void);
