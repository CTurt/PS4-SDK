#pragma once

#include "types.h"

/* Grabbed from libusb (www.libusb.org) */

enum libusb_class_code {
	LIBUSB_CLASS_PER_INTERFACE = 0,
	LIBUSB_CLASS_AUDIO = 1,
	LIBUSB_CLASS_COMM = 2,
	LIBUSB_CLASS_HID = 3,
	LIBUSB_CLASS_PHYSICAL = 5,
	LIBUSB_CLASS_PRINTER = 7,
	LIBUSB_CLASS_PTP = 6,
	LIBUSB_CLASS_IMAGE = 6,
	LIBUSB_CLASS_MASS_STORAGE = 8,
	LIBUSB_CLASS_HUB = 9,
	LIBUSB_CLASS_DATA = 10,
	LIBUSB_CLASS_SMART_CARD = 0x0b,
	LIBUSB_CLASS_CONTENT_SECURITY = 0x0d,
	LIBUSB_CLASS_VIDEO = 0x0e,
	LIBUSB_CLASS_PERSONAL_HEALTHCARE = 0x0f,
	LIBUSB_CLASS_DIAGNOSTIC_DEVICE = 0xdc,
	LIBUSB_CLASS_WIRELESS = 0xe0,
	LIBUSB_CLASS_APPLICATION = 0xfe,
	LIBUSB_CLASS_VENDOR_SPEC = 0xff
};

enum libusb_descriptor_type {
	LIBUSB_DT_DEVICE = 0x01,
	LIBUSB_DT_CONFIG = 0x02,
	LIBUSB_DT_STRING = 0x03,
	LIBUSB_DT_INTERFACE = 0x04,
	LIBUSB_DT_ENDPOINT = 0x05,
	LIBUSB_DT_BOS = 0x0f,
	LIBUSB_DT_DEVICE_CAPABILITY = 0x10,
	LIBUSB_DT_HID = 0x21,
	LIBUSB_DT_REPORT = 0x22,
	LIBUSB_DT_PHYSICAL = 0x23,
	LIBUSB_DT_HUB = 0x29,
	LIBUSB_DT_SUPERSPEED_HUB = 0x2a,
	LIBUSB_DT_SS_ENDPOINT_COMPANION = 0x30
};

#define LIBUSB_ENDPOINT_ADDRESS_MASK 0x0f
#define LIBUSB_ENDPOINT_DIR_MASK 0x80

enum libusb_endpoint_direction {
	LIBUSB_ENDPOINT_IN = 0x80,
	LIBUSB_ENDPOINT_OUT = 0x00
};

#define LIBUSB_TRANSFER_TYPE_MASK 0x03

enum libusb_transfer_type {
	LIBUSB_TRANSFER_TYPE_CONTROL = 0,
	LIBUSB_TRANSFER_TYPE_ISOCHRONOUS = 1,
	LIBUSB_TRANSFER_TYPE_BULK = 2,
	LIBUSB_TRANSFER_TYPE_INTERRUPT = 3,
	LIBUSB_TRANSFER_TYPE_BULK_STREAM = 4,
};

enum libusb_standard_request {
	LIBUSB_REQUEST_GET_STATUS = 0x00,
	LIBUSB_REQUEST_CLEAR_FEATURE = 0x01,
	LIBUSB_REQUEST_SET_FEATURE = 0x03,
	LIBUSB_REQUEST_SET_ADDRESS = 0x05,
	LIBUSB_REQUEST_GET_DESCRIPTOR = 0x06,
	LIBUSB_REQUEST_SET_DESCRIPTOR = 0x07,
	LIBUSB_REQUEST_GET_CONFIGURATION = 0x08,
	LIBUSB_REQUEST_SET_CONFIGURATION = 0x09,
	LIBUSB_REQUEST_GET_INTERFACE = 0x0A,
	LIBUSB_REQUEST_SET_INTERFACE = 0x0B,
	LIBUSB_REQUEST_SYNCH_FRAME = 0x0C,
	LIBUSB_REQUEST_SET_SEL = 0x30,
	LIBUSB_SET_ISOCH_DELAY = 0x31,
};

enum libusb_request_type {
	LIBUSB_REQUEST_TYPE_STANDARD = (0x00 << 5),
	LIBUSB_REQUEST_TYPE_CLASS = (0x01 << 5),
	LIBUSB_REQUEST_TYPE_VENDOR = (0x02 << 5),
	LIBUSB_REQUEST_TYPE_RESERVED = (0x03 << 5)
};

enum libusb_request_recipient {
	LIBUSB_RECIPIENT_DEVICE = 0x00,
	LIBUSB_RECIPIENT_INTERFACE = 0x01,
	LIBUSB_RECIPIENT_ENDPOINT = 0x02,
	LIBUSB_RECIPIENT_OTHER = 0x03,
};

enum libusb_error {
	LIBUSB_SUCCESS = 0,
	LIBUSB_ERROR_IO = -1,
	LIBUSB_ERROR_INVALID_PARAM = -2,
	LIBUSB_ERROR_ACCESS = -3,
	LIBUSB_ERROR_NO_DEVICE = -4,
	LIBUSB_ERROR_NOT_FOUND = -5,
	LIBUSB_ERROR_BUSY = -6,
	LIBUSB_ERROR_TIMEOUT = -7,
	LIBUSB_ERROR_OVERFLOW = -8,
	LIBUSB_ERROR_PIPE = -9,
	LIBUSB_ERROR_INTERRUPTED = -10,
	LIBUSB_ERROR_NO_MEM = -11,
	LIBUSB_ERROR_NOT_SUPPORTED = -12,
	LIBUSB_ERROR_OTHER = -99,
};

#define LIBUSB_ERROR_COUNT 14

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

struct libusb_endpoint_descriptor {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bEndpointAddress;
	uint8_t  bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t  bInterval;
	uint8_t  bRefresh;
	uint8_t  bSynchAddress;
	const unsigned char *extra;
	int extra_length;
};

struct libusb_interface_descriptor {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bInterfaceNumber;
	uint8_t  bAlternateSetting;
	uint8_t  bNumEndpoints;
	uint8_t  bInterfaceClass;
	uint8_t  bInterfaceSubClass;
	uint8_t  bInterfaceProtocol;
	uint8_t  iInterface;
	const struct libusb_endpoint_descriptor *endpoint;
	const unsigned char *extra;
	int extra_length;
};

struct libusb_interface {
	const struct libusb_interface_descriptor *altsetting;
	int num_altsetting;
};

struct libusb_config_descriptor {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint16_t wTotalLength;
	uint8_t  bNumInterfaces;
	uint8_t  bConfigurationValue;
	uint8_t  iConfiguration;
	uint8_t  bmAttributes;
	uint8_t  MaxPower;
	const struct libusb_interface *interface;
	const unsigned char *extra;
	int extra_length;
};

typedef struct libusb_device libusb_device;
typedef struct libusb_device_handle libusb_device_handle;

extern int (*sceUsbdInit)(void);
extern void (*sceUsbdExit)(void);

extern ssize_t (*sceUsbdGetDeviceList)(libusb_device ***list);
extern void (*sceUsbdFreeDeviceList)(libusb_device **list, int unrefDevices);

extern int (*sceUsbdGetDeviceDescriptor)(libusb_device *device, libusb_device_descriptor *desc);

extern int (*sceUsbdOpen)(libusb_device *dev, libusb_device_handle **devh);
extern libusb_device_handle *(*sceUsbdOpenDeviceWithVidPid)(unsigned short vendorId, unsigned short productId);
extern void (*sceUsbdClose)(libusb_device_handle *devh);

extern int (*sceUsbdSetInterfaceAltSetting)(libusb_device_handle *dev, int interface_number, int alternate_setting);
extern int (*sceUsbdClearHalt)(libusb_device_handle *devh, unsigned char endpoint);
extern int (*sceUsbdResetDevice)(libusb_device_handle *devh);
extern int (*sceUsbdCheckConnected)(libusb_device_handle *devh);

extern int (*sceUsbdControlTransfer)(libusb_device_handle *devh, uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout);
extern int (*sceUsbdBulkTransfer)(struct libusb_device_handle *devh, unsigned char endpoint, unsigned char *data, int length, int *transferred, unsigned int timeout);
extern int (*sceUsbdInterruptTransfer)(struct libusb_device_handle *devh, unsigned char endpoint, unsigned char *data, int length, int *transferred,  unsigned int timeout);

extern int (*sceUsbdGetActiveConfigDescriptor)(libusb_device *dev, struct libusb_config_descriptor **config);
extern int (*sceUsbdGetConfigDescriptor)(libusb_device *dev, uint8_t config_index, struct libusb_config_descriptor **config);
extern int (*sceUsbdGetConfigDescriptorByValue)(libusb_device *dev, uint8_t bConfigurationValue, struct libusb_config_descriptor **config);
extern void (*sceUsbdFreeConfigDescriptor)(struct libusb_config_descriptor *config);

void initUsb(void);
