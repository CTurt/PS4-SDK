#include "ps4.h"

unsigned int *framebuffer = NULL;

void getFramebuffer(void) {
	struct memoryRegionInfo info;
	struct otherMemoryRegionInfo otherInfo;
	
	void *m = NULL;
	
	int i;
	for(i = 0; i < 16; i++) {
		// Find base of next mapping
		getOtherMemoryInfo(m, 1, &otherInfo);
		
		// Get more info about this mapping
		getMemoryInfo(otherInfo.base, &info);
		
		// Search mappings 14 and 15 if they are readable
		if((i == 14 || i == 15) && (info.flags & PROT_CPU_READ)) {
			framebuffer = info.base;
			
			// Search for the colour we filled the framebuffer with
			while(framebuffer < (unsigned int *)info.end) {
				if(
					framebuffer[0] == 0xffc2c2c2 &&
					framebuffer[1] == 0xffc2c2c2
				) break;
				
				framebuffer++;
			}
		}
		
		m = info.end;
	}
}

void *canvasRenderer(void *arg) {
	getFramebuffer();
	
	unsigned int colour = RED;
	
	while(1) {
		int x, y;
		for(x = 0; x < 160; x++) {
			for(y = 0; y < 144 + 16; y++) {
				framebuffer[x + y * 144] = colour;
				
				colour = ((colour + 1) & 0x00ffffff) | (0xff << 24);
			}
		}
	}
	
	return NULL;
}

int _main(void) {
	// Init and resolve libraries
	initKernel();
	
	initPthread();
	
	
	// Create our canvas rendering thread
	ScePthread thread;
	scePthreadCreate(&thread, NULL, canvasRenderer, NULL, "canvasRenderer");
	
	
	// Let this thread get back to updating the JS engine
	return 0;
}
