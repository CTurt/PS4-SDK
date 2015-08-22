#include "module.h"

#include "usb.h"

void initUsb(void) {
	int libUsb;
	char libUsbName[] = "libSceUsbd.sprx";
	loadModule(libUsbName, &libUsb);
	
	RESOLVE(libUsb, sceUsbdInit);
	RESOLVE(libUsb, sceUsbdExit);
	RESOLVE(libUsb, sceUsbdGetDeviceList);
	RESOLVE(libUsb, sceUsbdFreeDeviceList);
	RESOLVE(libUsb, sceUsbdGetDeviceDescriptor);
}
