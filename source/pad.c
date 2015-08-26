#include "module.h"

#include "pad.h"

int (*scePadInit)(void);
int (*scePadOpen)(int, int, int, void *);
int (*scePadRead)(int handle, void *data, int count);

void initPad(void) {
	int libPad;
	loadModule("libScePad.sprx", &libPad);
	
	RESOLVE(libPad, scePadInit);
	RESOLVE(libPad, scePadOpen);
	RESOLVE(libPad, scePadRead);
	
	scePadInit();
}
