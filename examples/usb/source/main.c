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

#define debug(sock, format, ...)\
	do {\
		char buffer[512];\
		int size = sprintf(buffer, format, ##__VA_ARGS__);\
		sceNetSend(sock, buffer, size, 0);\
	} while(0)

int _main(void) {
	// Load modules
	initLibc();
	initNetwork();
	initUsb();
	
	
	// Connect to server
	char socketName[] = "debug";
	
	struct sockaddr_in server;
	
	server.sin_family = htons(AF_INET);
	server.sin_addr.s_addr = IP(192, 168, 0, 4);
	server.sin_port = htons(9023);
	memset(server.sin_zero, 0, sizeof(server.sin_zero));
	
	int sock = sceNetSocket(socketName, AF_INET, SOCK_STREAM, 0);
	sceNetConnect(sock, (struct sockaddr *)&server, sizeof(server));
	
	
	// Read all USBs
	libusb_device **list;
	libusb_device_descriptor ddesc;
	
	int ret = sceUsbdInit();
	debug(sock, "Init %d\n", ret);
	
	int count = sceUsbdGetDeviceList(&list);
	debug(sock, "Get device list %d\n", count);
	
	int i;
	for(i = 0; i < count; i++) {
		ret = sceUsbdGetDeviceDescriptor(list[i], &ddesc);
		debug(sock, "Get descriptor %d\n", ret);
		
		unsigned short vendor = ddesc.idVendor;
		unsigned short product = ddesc.idProduct;
		
		debug(sock, "Vendor: %d, Product: %d\n", vendor, product);
		
		libusb_device_handle *handle;
		//ret = sceUsbdOpen(list[i], &handle);
		handle = sceUsbdOpenDeviceWithVidPid(vendor, product);
		
		debug(sock, "Open 0x%p\n", handle);
		
		unsigned char data[512];
		int length;
		ret = sceUsbdBulkTransfer(handle, LIBUSB_ENDPOINT_IN, data, sizeof(data), &length, 0);
		
		debug(sock, "Transfer 0x%08x, length %d\n", ret, length);
		
		sceNetSend(sock, data, length, 0);
	}
	
	sceUsbdFreeDeviceList(list, 1);
	
	
	// Return to browser
	sceUsbdExit();
	sceNetSocketClose(sock);
	
	return 0;
}
