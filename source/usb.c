#include "module.h"

#include "usb.h"

int (*sceUsbdInit)(void);
void (*sceUsbdExit)(void);
ssize_t (*sceUsbdGetDeviceList)(libusb_device ***list);
void (*sceUsbdFreeDeviceList)(libusb_device **list, int unrefDevices);
int (*sceUsbdGetDeviceDescriptor)(libusb_device *device, libusb_device_descriptor *desc);

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
