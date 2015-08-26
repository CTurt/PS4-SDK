#include "module.h"

#include "pad.h"

void initPad(void) {
	int libPad;
	loadModule("libScePad.sprx", &libPad);
}
