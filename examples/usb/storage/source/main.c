#include "ps4.h"

int netdbg_sock;

#define debug(...) \
	do { \
		char _debug_buffer[512]; \
		int _dbg_size = sprintf(_debug_buffer, ##__VA_ARGS__); \
		sceNetSend(netdbg_sock, _debug_buffer, _dbg_size, 0); \
	} while(0)


/* Grabbed from libusb/examples/xusb.c */

#define ERR_EXIT(errcode) do { debug("   %i\n", errcode); return -1; } while (0)
#define CALL_CHECK(fcall) do { r=fcall; if (r < 0) ERR_EXIT(r); } while (0);
#define B(x) (((x)!=0)?1:0)
#define be_to_int32(buf) (((buf)[0]<<24)|((buf)[1]<<16)|((buf)[2]<<8)|(buf)[3])

#define RETRY_MAX                     5
#define REQUEST_SENSE_LENGTH          0x12
#define INQUIRY_LENGTH                0x24
#define READ_CAPACITY_LENGTH          0x08

// Mass Storage Requests values. See section 3 of the Bulk-Only Mass Storage Class specifications
#define BOMS_RESET                    0xFF
#define BOMS_GET_MAX_LUN              0xFE

// Section 5.1: Command Block Wrapper (CBW)
struct command_block_wrapper {
	uint8_t dCBWSignature[4];
	uint32_t dCBWTag;
	uint32_t dCBWDataTransferLength;
	uint8_t bmCBWFlags;
	uint8_t bCBWLUN;
	uint8_t bCBWCBLength;
	uint8_t CBWCB[16];
};

// Section 5.2: Command Status Wrapper (CSW)
struct command_status_wrapper {
	uint8_t dCSWSignature[4];
	uint32_t dCSWTag;
	uint32_t dCSWDataResidue;
	uint8_t bCSWStatus;
};

static uint8_t cdb_length[256] = {
//	 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
	06,06,06,06,06,06,06,06,06,06,06,06,06,06,06,06,  //  0
	06,06,06,06,06,06,06,06,06,06,06,06,06,06,06,06,  //  1
	10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,  //  2
	10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,  //  3
	10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,  //  4
	10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,  //  5
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,  //  6
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,  //  7
	16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,  //  8
	16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,  //  9
	12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,  //  A
	12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,  //  B
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,  //  C
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,  //  D
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,  //  E
	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,  //  F
};

static void display_buffer_hex(unsigned char *buffer, unsigned size)
{
	unsigned i, j, k;

	for (i=0; i<size; i+=16) {
		debug("\n  %08x  ", i);
		for(j=0,k=0; k<16; j++,k++) {
			if (i+j < size) {
				debug("%02x", buffer[i+j]);
			} else {
				debug("  ");
			}
			debug(" ");
		}
		debug(" ");
		for(j=0,k=0; k<16; j++,k++) {
			if (i+j < size) {
				if ((buffer[i+j] < 32) || (buffer[i+j] > 126)) {
					debug(".");
				} else {
					debug("%c", buffer[i+j]);
				}
			}
		}
	}
	debug("\n" );
}

static int send_mass_storage_command(libusb_device_handle *handle, uint8_t endpoint, uint8_t lun,
	uint8_t *cdb, uint8_t direction, int data_length, uint32_t *ret_tag)
{
	static uint32_t tag = 1;
	uint8_t cdb_len;
	int i, r, size;
	struct command_block_wrapper cbw;

	if (cdb == NULL) {
		return -1;
	}

	if (endpoint & LIBUSB_ENDPOINT_IN) {
		debug("send_mass_storage_command: cannot send command on IN endpoint\n");
		return -1;
	}

	cdb_len = cdb_length[cdb[0]];
	if ((cdb_len == 0) || (cdb_len > sizeof(cbw.CBWCB))) {
		debug("send_mass_storage_command: don't know how to handle this command (%02X, length %d)\n",
			cdb[0], cdb_len);
		return -1;
	}

	memset(&cbw, 0, sizeof(cbw));
	cbw.dCBWSignature[0] = 'U';
	cbw.dCBWSignature[1] = 'S';
	cbw.dCBWSignature[2] = 'B';
	cbw.dCBWSignature[3] = 'C';
	*ret_tag = tag;
	cbw.dCBWTag = tag++;
	cbw.dCBWDataTransferLength = data_length;
	cbw.bmCBWFlags = direction;
	cbw.bCBWLUN = lun;
	// Subclass is 1 or 6 => cdb_len
	cbw.bCBWCBLength = cdb_len;
	memcpy(cbw.CBWCB, cdb, cdb_len);

	i = 0;
	do {
		// The transfer length must always be exactly 31 bytes.
		r = sceUsbdBulkTransfer(handle, endpoint, (unsigned char*)&cbw, 31, &size, 1000);
		if (r == LIBUSB_ERROR_PIPE) {
			sceUsbdClearHalt(handle, endpoint);
		}
		i++;
	} while ((r == LIBUSB_ERROR_PIPE) && (i<RETRY_MAX));
	if (r != LIBUSB_SUCCESS) {
		debug("   send_mass_storage_command\n");
		return -1;
	}

	debug("   sent %d CDB bytes\n", cdb_len);
	return 0;
}


static int get_mass_storage_status(libusb_device_handle *handle, uint8_t endpoint, uint32_t expected_tag)
{
	int i, r, size;
	struct command_status_wrapper csw;

	// The device is allowed to STALL this transfer. If it does, you have to
	// clear the stall and try again.
	i = 0;
	do {
		r = sceUsbdBulkTransfer(handle, endpoint, (unsigned char*)&csw, 13, &size, 1000);
		if (r == LIBUSB_ERROR_PIPE) {
			sceUsbdClearHalt(handle, endpoint);
		}
		i++;
	} while ((r == LIBUSB_ERROR_PIPE) && (i<RETRY_MAX));
	if (r != LIBUSB_SUCCESS) {
		debug("   get_mass_storage_status\n");
		return -1;
	}
	if (size != 13) {
		debug("   get_mass_storage_status: received %d bytes (expected 13)\n", size);
		return -1;
	}
	if (csw.dCSWTag != expected_tag) {
		debug("   get_mass_storage_status: mismatched tags (expected %08X, received %08X)\n",
			expected_tag, csw.dCSWTag);
		return -1;
	}
	// For this test, we ignore the dCSWSignature check for validity...
	debug("   Mass Storage Status: %02X (%s)\n", csw.bCSWStatus, csw.bCSWStatus?"FAILED":"Success");
	if (csw.dCSWTag != expected_tag)
		return -1;
	if (csw.bCSWStatus) {
		// REQUEST SENSE is appropriate only if bCSWStatus is 1, meaning that the
		// command failed somehow.  Larger values (2 in particular) mean that
		// the command couldn't be understood.
		if (csw.bCSWStatus == 1)
			return -2;	// request Get Sense
		else
			return -1;
	}

	// In theory we also should check dCSWDataResidue.  But lots of devices
	// set it wrongly.
	return 0;
}

static void get_sense(libusb_device_handle *handle, uint8_t endpoint_in, uint8_t endpoint_out)
{
	uint8_t cdb[16];	// SCSI Command Descriptor Block
	uint8_t sense[18];
	uint32_t expected_tag;
	int size;
	int rc;

	// Request Sense
	debug("Request Sense:\n");
	memset(sense, 0, sizeof(sense));
	memset(cdb, 0, sizeof(cdb));
	cdb[0] = 0x03;	// Request Sense
	cdb[4] = REQUEST_SENSE_LENGTH;

	send_mass_storage_command(handle, endpoint_out, 0, cdb, LIBUSB_ENDPOINT_IN, REQUEST_SENSE_LENGTH, &expected_tag);
	rc = sceUsbdBulkTransfer(handle, endpoint_in, (unsigned char*)&sense, REQUEST_SENSE_LENGTH, &size, 1000);
	if (rc < 0)
	{
		debug("sceUsbdBulkTransfer failed\n");
		return;
	}
	debug("   received %d bytes\n", size);

	if ((sense[0] != 0x70) && (sense[0] != 0x71)) {
		debug("   ERROR No sense data\n");
	} else {
		debug("   ERROR Sense: %02X %02X %02X\n", sense[2]&0x0F, sense[12], sense[13]);
	}
	// Strictly speaking, the get_mass_storage_status() call should come
	// before these debug() lines.  If the status is nonzero then we must
	// assume there's no data in the buffer.  For xusb it doesn't matter.
	get_mass_storage_status(handle, endpoint_in, expected_tag);
}

// Mass Storage device to test bulk transfers (non destructive test)
static int test_mass_storage(libusb_device_handle *handle, uint8_t endpoint_in, uint8_t endpoint_out)
{
	int r, size;
	uint8_t lun;
	uint32_t expected_tag;
	uint32_t i, max_lba, block_size;
	double device_size;
	uint8_t cdb[16];	// SCSI Command Descriptor Block
	uint8_t buffer[64];
	char vid[9], pid[9], rev[5];
	unsigned char *data;

	debug("Reading Max LUN:\n");
	r = sceUsbdControlTransfer(handle, LIBUSB_ENDPOINT_IN|LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE,
		BOMS_GET_MAX_LUN, 0, 0, &lun, 1, 1000);
	// Some devices send a STALL instead of the actual value.
	// In such cases we should set lun to 0.
	if (r == 0) {
		lun = 0;
	} else if (r < 0) {
		debug("   Failed\n");
	}
	debug("   Max LUN = %d\n", lun);

	// Send Inquiry
	debug("Sending Inquiry:\n");
	memset(buffer, 0, sizeof(buffer));
	memset(cdb, 0, sizeof(cdb));
	cdb[0] = 0x12;	// Inquiry
	cdb[4] = INQUIRY_LENGTH;

	send_mass_storage_command(handle, endpoint_out, lun, cdb, LIBUSB_ENDPOINT_IN, INQUIRY_LENGTH, &expected_tag);
	CALL_CHECK(sceUsbdBulkTransfer(handle, endpoint_in, (unsigned char*)&buffer, INQUIRY_LENGTH, &size, 1000));
	debug("   received %d bytes\n", size);
	// The following strings are not zero terminated
	for (i=0; i<8; i++) {
		vid[i] = buffer[8+i];
		pid[i] = buffer[16+i];
		rev[i/2] = buffer[32+i/2];	// instead of another loop
	}
	vid[8] = 0;
	pid[8] = 0;
	rev[4] = 0;
	debug("   VID:PID:REV \"%8s\":\"%8s\":\"%4s\"\n", vid, pid, rev);
	if (get_mass_storage_status(handle, endpoint_in, expected_tag) == -2) {
		get_sense(handle, endpoint_in, endpoint_out);
	}

	// Read capacity
	debug("Reading Capacity:\n");
	memset(buffer, 0, sizeof(buffer));
	memset(cdb, 0, sizeof(cdb));
	cdb[0] = 0x25;	// Read Capacity

	send_mass_storage_command(handle, endpoint_out, lun, cdb, LIBUSB_ENDPOINT_IN, READ_CAPACITY_LENGTH, &expected_tag);
	CALL_CHECK(sceUsbdBulkTransfer(handle, endpoint_in, (unsigned char*)&buffer, READ_CAPACITY_LENGTH, &size, 1000));
	debug("   received %d bytes\n", size);
	max_lba = be_to_int32(&buffer[0]);
	block_size = be_to_int32(&buffer[4]);
	device_size = ((double)(max_lba+1))*block_size/(1024*1024*1024);
	debug("   Max LBA: %08X, Block Size: %08X (%.2f GB)\n", max_lba, block_size, device_size);
	if (get_mass_storage_status(handle, endpoint_in, expected_tag) == -2) {
		get_sense(handle, endpoint_in, endpoint_out);
	}

	// coverity[tainted_data]
	data = (unsigned char*) calloc(1, block_size);
	if (data == NULL) {
		debug("   unable to allocate data buffer\n");
		return -1;
	}

	// Send Read
	debug("Attempting to read %d bytes:\n", block_size);
	memset(cdb, 0, sizeof(cdb));

	cdb[0] = 0x28;	// Read(10)
	cdb[8] = 0x01;	// 1 block

	send_mass_storage_command(handle, endpoint_out, lun, cdb, LIBUSB_ENDPOINT_IN, block_size, &expected_tag);
	sceUsbdBulkTransfer(handle, endpoint_in, data, block_size, &size, 5000);
	debug("   READ: received %d bytes\n", size);
	if (get_mass_storage_status(handle, endpoint_in, expected_tag) == -2) {
		get_sense(handle, endpoint_in, endpoint_out);
	} else {
		display_buffer_hex(data, size);
	}
	free(data);

	return 0;
}

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

	netdbg_sock = sceNetSocket("netdebug", AF_INET, SOCK_STREAM, 0);
	sceNetConnect(netdbg_sock, (struct sockaddr *)&server, sizeof(server));

	uint8_t endpoint_in = 0, endpoint_out = 0;
	int usb_storage_found = 0;
	unsigned short usb_stor_vid;
	unsigned short usb_stor_pid;

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

				if ( (interdesc->bInterfaceClass == LIBUSB_CLASS_MASS_STORAGE)
					&& ( (interdesc->bInterfaceSubClass == 0x01)
					|| (interdesc->bInterfaceSubClass == 0x06) )
					&& (interdesc->bInterfaceProtocol == 0x50) ) {
						usb_storage_found = 1;
						usb_stor_vid = desc.idVendor;
						usb_stor_pid = desc.idProduct;
				}

				int l;
				for(l = 0; l < interdesc->bNumEndpoints; l++) {
					debug("      Endpoint %i\n", l);
					epdesc = &interdesc->endpoint[l];

					debug("        Descriptor Type: 0x%02X\n", epdesc->bDescriptorType);
					debug("        EP Address: 0x%02X\n", epdesc->bEndpointAddress);
					debug("        EP Attributes: 0x%02X\n", epdesc->bmAttributes);

					if ((epdesc->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK) & (LIBUSB_TRANSFER_TYPE_BULK | LIBUSB_TRANSFER_TYPE_INTERRUPT)) {
						if (epdesc->bEndpointAddress & LIBUSB_ENDPOINT_IN) {
							if (!endpoint_in)
								endpoint_in = epdesc->bEndpointAddress;
						} else {
							if (!endpoint_out)
								endpoint_out = epdesc->bEndpointAddress;
						}
					}
				}
			}
		}

		sceUsbdFreeConfigDescriptor(config);
	}

	sceUsbdFreeDeviceList(list, 1);

	if (usb_storage_found) {
		libusb_device_handle *handle;
		handle = sceUsbdOpenDeviceWithVidPid(usb_stor_vid, usb_stor_pid);
		test_mass_storage(handle, endpoint_in, endpoint_out);
	} else {
		debug("USB storage not found.\n");
	}

	// Return to browser
	sceUsbdExit();
	sceNetSocketClose(netdbg_sock);

	return 0;
}
