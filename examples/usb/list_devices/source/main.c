/*
Gives the following output, when a USB flash drive is inserted into slot 0

sceUsbdInit 0
Device list count 1
Device 0
  Get device descriptor: 0
  Device Class: 0x00
  Vendor ID: 0x0930
  Product ID: 0x6533
  Number of possible configurations: 1

  Get config descriptor: 0
  Number of interfaces: 1

  Interface 0
    Number of alternate settings: 1
    Interface 0
      Interface Number: 0
      Number of endpoints: 3
      Endpoint 0
        Descriptor Type: 0x05
        EP Address: 0x81
        EP Attributes: 0x02
      Endpoint 1
        Descriptor Type: 0x05
        EP Address: 0x02
        EP Attributes: 0x02
      Endpoint 2
        Descriptor Type: 0x05
        EP Address: 0x83
        EP Attributes: 0x03
*/

#include "ps4.h"

#define debug(...) \
	do { \
		char debug_buffer[512]; \
		int size = sprintf(debug_buffer, ##__VA_ARGS__); \
		sceNetSend(netdbg_sock, debug_buffer, size, 0); \
	} while(0)

int _main(void) {
	// Load modules
	initKernel();
	
	initLibc();
	initNetwork();
	initUsb();

	// Init netdebug
	struct sockaddr_in server;
	server.sin_len = sizeof(server);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = IP(192, 168, 0, 4);
	server.sin_port = sceNetHtons(9023);
	memset(server.sin_zero, 0, sizeof(server.sin_zero));

	int netdbg_sock = sceNetSocket("netdebug", AF_INET, SOCK_STREAM, 0);
	sceNetConnect(netdbg_sock, (struct sockaddr *)&server, sizeof(server));


	// Read all USBs
	libusb_device **list;
	libusb_device_descriptor desc;
	struct libusb_config_descriptor *config;
	const struct libusb_interface *inter;
	const struct libusb_interface_descriptor *interdesc;
	const struct libusb_endpoint_descriptor *epdesc;
	libusb_device_handle *handle;
	
	int ret = sceUsbdInit();
	debug("sceUsbdInit %d\n", ret);

	int count = sceUsbdGetDeviceList(&list);
	debug("Device list count %d\n", count);

	int i;
	for(i = 0; i < count; i++) {
		debug("Device %i\n", i);

		ret = sceUsbdGetDeviceDescriptor(list[i], &desc);
		debug("  Get device descriptor: %d\n", ret);

		debug("  Device Class: 0x%02X\n", desc.bDeviceClass);
		debug("  Vendor ID: 0x%04X\n", desc.idVendor);
		debug("  Product ID: 0x%04X\n", desc.idProduct);
		debug("  Number of possible configurations: %i\n", desc.bNumConfigurations);

		//ret = sceUsbdOpen(list[i], &handle);
		//handle = sceUsbdOpenDeviceWithVidPid(vendor, product);
		
		ret = sceUsbdGetConfigDescriptor(list[i], 0, &config);
		debug("\n  Get config descriptor: %d\n", ret);

		debug("  Number of interfaces: %i\n\n", config->bNumInterfaces);

		int j;
		for (j = 0; j < config->bNumInterfaces; j++) {
			debug("  Interface %i\n", j);
			inter = &config->interface[j];

			debug("    Number of alternate settings: %i\n", inter->num_altsetting);

			int k;
			for (k = 0; k < inter->num_altsetting; k++) {
				debug("    Interface %i\n", j);
				interdesc = &inter->altsetting[j];

				debug("      Interface Number: %i\n", interdesc->bInterfaceNumber);
				debug("      Number of endpoints: %i\n", interdesc->bNumEndpoints);

				int l;
				for(l = 0; l < interdesc->bNumEndpoints; l++) {
					debug("      Endpoint %i\n", l);
					epdesc = &interdesc->endpoint[l];

					debug("        Descriptor Type: 0x%02X\n", epdesc->bDescriptorType);
					debug("        EP Address: 0x%02X\n", epdesc->bEndpointAddress);
					debug("        EP Attributes: 0x%02X\n", epdesc->bmAttributes);
				}
			}
		}

		sceUsbdFreeConfigDescriptor(config);
	}

	sceUsbdFreeDeviceList(list, 1);

	// Return to browser
	sceUsbdExit();
	sceNetSocketClose(netdbg_sock);

	return 0;
}
