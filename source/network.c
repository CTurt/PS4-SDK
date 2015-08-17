#include "network.h"

#include "module.h"

int (*socket)(const char *, int, int, int);
int (*connect)(int, struct sockaddr_in *, int);
int (*send)(int, const void *, size_t, int);
int (*close)(int);

void initNetwork(void) {
	RESOLVE(SCENET, socket);
	RESOLVE(SCENET, connect);
	RESOLVE(SCENET, send);
	RESOLVE(SCENET, close);
}
