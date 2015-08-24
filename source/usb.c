#include "module.h"

#include "usb.h"

int (*sceUsbdInit)(void);
void (*sceUsbdExit)(void);
ssize_t (*sceUsbdGetDeviceList)(libusb_device ***list);
void (*sceUsbdFreeDeviceList)(libusb_device **list, int unrefDevices);
int (*sceUsbdGetDeviceDescriptor)(libusb_device *device, libusb_device_descriptor *desc);
int (*sceUsbdOpen)(libusb_device *dev, libusb_device_handle **devh);
libusb_device_handle *(*sceUsbdOpenDeviceWithVidPid)(unsigned short vendorId, unsigned short productId);
int (*sceUsbdBulkTransfer)(libusb_device_handle *devh, unsigned char endpoint, unsigned char *data, int length, int *transferred, unsigned int timeout);

void initUsb(void) {
	int libUsb;
	loadModule("libSceUsbd.sprx", &libUsb);
	
	RESOLVE(libUsb, sceUsbdInit);
	RESOLVE(libUsb, sceUsbdExit);
	RESOLVE(libUsb, sceUsbdGetDeviceList);
	RESOLVE(libUsb, sceUsbdFreeDeviceList);
	RESOLVE(libUsb, sceUsbdGetDeviceDescriptor);
	RESOLVE(libUsb, sceUsbdOpen);
	RESOLVE(libUsb, sceUsbdOpenDeviceWithVidPid);
	RESOLVE(libUsb, sceUsbdBulkTransfer);
}
