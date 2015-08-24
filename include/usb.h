#pragma once

#include "types.h"

typedef struct libusb_device libusb_device;
typedef struct libusb_device_handle libusb_device_handle;

typedef struct libusb_device_descriptor {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t bcdUSB;
	uint8_t bDeviceClass;
	uint8_t bDeviceSubClass;
	uint8_t bDeviceProtocol;
	uint8_t bMaxPacketSize0;
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t bcdDevice;
	uint8_t iManufacturer;
	uint8_t iProduct;
	uint8_t iSerialNumber;
	uint8_t bNumConfigurations;
} libusb_device_descriptor;

enum libusb_endpoint_direction {
	LIBUSB_ENDPOINT_IN = 0x80,
	LIBUSB_ENDPOINT_OUT = 0x00
};

extern int (*sceUsbdInit)(void);
extern void (*sceUsbdExit)(void);
extern ssize_t (*sceUsbdGetDeviceList)(libusb_device ***list);
extern void (*sceUsbdFreeDeviceList)(libusb_device **list, int unrefDevices);
extern int (*sceUsbdGetDeviceDescriptor)(libusb_device *device, libusb_device_descriptor *desc);
extern int (*sceUsbdOpen)(libusb_device *dev, libusb_device_handle **devh);
extern libusb_device_handle *(*sceUsbdOpenDeviceWithVidPid)(unsigned short vendorId, unsigned short productId);
extern int (*sceUsbdBulkTransfer)(struct libusb_device_handle *devh, unsigned char endpoint, unsigned char *data, int length, int *transferred, unsigned int timeout);

void initUsb(void);
