#include "kernel.h"
#include "module.h"

#include "camera.h"

int (*sceCameraOpen)(int userid, int type, int index, void *);
int (*sceCameraClose)(int handle);
int (*sceCameraIsAttached)(int index);
int (*sceCameraGetFrameData)(int handle, SceCameraFrameData *frame);
int (*sceCameraStart)(int handle, SceCameraStartParameter *param);
int (*sceCameraStop)(int handle);
int (*sceCameraGetDeviceInfo)(int handle, SceCameraDeviceInfo *info);
int (*sceCameraGetDeviceConfig)(int handle, SceCameraConfig *config);
int (*sceCameraGetConfig)(int handle, SceCameraConfig *config);
int (*sceCameraSetConfig)(int handle, SceCameraConfig *config);

void initCamera(void) {
	int libCamera = sceKernelLoadStartModule("libSceCamera.sprx", 0, NULL, 0, 0, 0);
	
	RESOLVE(libCamera, sceCameraOpen);
	RESOLVE(libCamera, sceCameraClose);
	RESOLVE(libCamera, sceCameraIsAttached);
	RESOLVE(libCamera, sceCameraGetFrameData);
	RESOLVE(libCamera, sceCameraStart);
	RESOLVE(libCamera, sceCameraStop);
	RESOLVE(libCamera, sceCameraGetDeviceInfo);
	RESOLVE(libCamera, sceCameraGetDeviceConfig);
	RESOLVE(libCamera, sceCameraGetConfig);
	RESOLVE(libCamera, sceCameraSetConfig);
}
