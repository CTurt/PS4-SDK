#include "module.h"

#include "pad.h"

int (*scePadInit)(void);

void initPad(void) {
	int libPad;
	loadModule("libScePad.sprx", &libPad);
	
	RESOLVE(libPad, scePadInit);
	
	scePadInit();
}
