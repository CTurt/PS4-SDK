#include "ps4.h"

#define debug(sock, ...) \
	do { \
		char buffer[512]; \
		int size = sprintf(buffer, ##__VA_ARGS__); \
		sceNetSend(sock, buffer, size, 0); \
	} while(0)

int _main(void) {
	// Init and resolve libraries
	initKernel();
	
	initLibc();
	initNetwork();
	
	
	// Connect to server
	char socketName[] = "debug";
	
	struct sockaddr_in server;
	
	server.sin_len = sizeof(server);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = IP(192, 168, 0, 4);
	server.sin_port = sceNetHtons(9023);
	memset(server.sin_zero, 0, sizeof(server.sin_zero));
	
	int sock = sceNetSocket(socketName, AF_INET, SOCK_STREAM, 0);
	sceNetConnect(sock, (struct sockaddr *)&server, sizeof(server));
	
	
	// Get font path
	char path[256] = "/";
	int length = 11;
	getSandboxDirectory(path + 1, &length);
	strcpy(path + 11, "/common/font/DFHEI5-SONY.ttf");
	
	
	// Open for reading, and get size
	int fd = open(path, O_RDONLY, 0);
	
	struct stat s;
	fstat(fd, &s);
	
	
	// Allocate buffer and read
	char *buffer = malloc(s.st_size);
	
	read(fd, buffer, s.st_size);
	
	close(fd);
	
	
	// Send
	sceNetSend(sock, buffer, s.st_size, 0);
	
	free(buffer);
	
	sceNetSocketClose(sock);
	
	
	// Return to browser
	return 0;
}
