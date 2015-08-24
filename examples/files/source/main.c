#include "ps4.h"

#define debug(sock, ...) \
	do { \
		char buffer[512]; \
		int size = sprintf(buffer, ##__VA_ARGS__); \
		sceNetSend(sock, buffer, size, 0); \
	} while(0)

int _main(void) {
	// Init and resolve libraries
	initLibc();
	initNetwork();

	// Connect to server and send message
	char socketName[] = "debug";

	struct sockaddr_in server;

	server.sin_family = sceNetHtons(AF_INET);
	server.sin_addr.s_addr = IP(192, 168, 0, 4);
	server.sin_port = sceNetHtons(9023);
	memset(server.sin_zero, 0, sizeof(server.sin_zero));

	int sock = sceNetSocket(socketName, AF_INET, SOCK_STREAM, 0);
	sceNetConnect(sock, (struct sockaddr *)&server, sizeof(server));


	char buffer[512];
	struct dirent *dent;

	int dfd = open("/", O_RDONLY, 0);
	if(dfd < 0) {
		debug(sock, "Invalid directory.\n");
		return 1;
	}

	while(getdents(dfd, buffer, sizeof(buffer)) != 0) {
        dent = (struct dirent *)buffer;

        while(dent->d_fileno) {
                debug(sock, "%s\n", dent->d_name);

                dent = (struct dirent *)((void *)dent + dent->d_reclen);
        }

		memset(buffer, 0, sizeof(buffer));
	}


	// Return to browser
	close(dfd);
	sceNetSocketClose(sock);

	return 0;
}
