#include "ps4.h"

unsigned char data[512];

int _main(void) {
	// Init and resolve libraries
	initPad();
	
	
	// Browser /user/home/ to see all user IDs (in hex)
	int userID = 0x10000000;
	
	int pad = scePadOpen(userID, 0, 0, NULL);
	
	scePadRead(pad, data, 1);
	
	scePadClose(pad);
	
	
	// Return to browser
	return 0;
}
