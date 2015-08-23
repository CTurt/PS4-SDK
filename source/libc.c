#include "module.h"

#include "libc.h"

void *(*memset)(void *destination, int value, size_t num);
void *(*memcpy)(void *destination, const void *source, size_t num);
char *(*strcpy)(char *destination, const char *source);
int (*sprintf)(char *str, const char *format, ...);

void initLibc(void) {
	int libc;
	loadModule("libSceLibcInternal.sprx", &libc);
	
	RESOLVE(libc, memset);
	RESOLVE(libc, memcpy);
	RESOLVE(libc, strcpy);
	RESOLVE(libc, sprintf);
}
