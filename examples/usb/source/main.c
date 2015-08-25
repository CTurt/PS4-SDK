/*
Gives the following output, when a USB flash drive is inserted into slot 0

Init 0
Get device list 1
Get descriptor 0
Vendor: 2352, Product: 25907
Open 0x8803c8220
Transfer 0x802400ff, length 0
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
	initLibc();
	initNetwork();
	initUsb();

	// Init netdebug
	struct sockaddr_in server;
	server.sin_family = sceNetHtons(AF_INET);
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
