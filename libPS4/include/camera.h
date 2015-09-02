#pragma once

#include "types.h"

//from libSceCamera.sprx firmware 1.76. Other can change 
typedef struct SceCameraStartParameter {
	uint32_t size; //0x0  <- set to  0x18 before call 24 bytes it is a check hardcoded in libSceCamera.sprx
	uint32_t unknown1;//0x4
	uint32_t unknown2;//0x8
	void * unknown3;//0xc
} SceCameraStartParameter;

typedef struct SceCameraFrameData {
	uint32_t size; //0x0  <- set to  size< 0xb1+0x158=0x209 (521) it is a check hardcoded in libSceCamera.sprx.I suppose size 520 it's the worst case to pass the check
	uint32_t unknown1;//0x4
	uint32_t unknown2[32];//0x8  
	void* pleft[4];//0x88 video frame pointers for left camera 4 resolution modes
	void* pright[4];//0xa8 video frame pointers for right camera 4 resolution modes
	uint32_t sizeleft[4]; //0xc8 video frame size for left camera 4 resolution modes
	uint32_t sizeright[4];//0xd8 video frame size for right camera 4 resolution modes
	uint32_t statusleft;//0xe8
	uint32_t statusright;//0xec
	uint32_t unknown3[70];//0xf0
} SceCameraFrameData;

typedef struct SceCameraDeviceInfo {
	uint32_t size; //0x0  <- set to 0x10 before call 24 bytes it is a check hardcoded in libSceCamera.sprx
	uint32_t revision;//0x4  <- check set to 0x1 before call
	uint32_t unknown1;//0x8
	uint32_t unknown2;//0xc	
} SceCameraDeviceInfo;

typedef struct SceCameraConfig {
	uint32_t size; //0x0  <- set to  0x68 it is a check hardcoded in libSceCamera.sprx
	uint32_t unknown[100]; //0x4
} SceCameraConfig;

extern int (*sceCameraOpen)(int userid, int type, int index, void *);
extern int (*sceCameraClose)(int handle);
extern int (*sceCameraIsAttached)(int index);
extern int (*sceCameraGetFrameData)(int handle, SceCameraFrameData *frame);
extern int (*sceCameraStart)(int handle, SceCameraStartParameter *param);
extern int (*sceCameraStop)(int handle);
extern int (*sceCameraGetDeviceInfo)(int handle, SceCameraDeviceInfo *info);
extern int (*sceCameraGetDeviceConfig)(int handle, SceCameraConfig *config);
extern int (*sceCameraGetConfig)(int handle, SceCameraConfig *config);
extern int (*sceCameraSetConfig)(int handle, SceCameraConfig *config);

void initCamera(void);
