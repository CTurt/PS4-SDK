#ifndef __FLASH_DRIVE_H__
#define __FLASH_DRIVE_H__

// flash_init() Error Return Values
#define FLASH_DRIVE_INIT_SUCCESS							0
#define FLASH_DRIVE_INIT_ERR_LIBUSB_INIT					1
#define FLASH_DRIVE_INIT_ERR_LIBUSB_OPEN_DEVICE				2
#define FLASH_DRIVE_INIT_ERR_LIBUSB_DETTACH_KERNEL_DRIVER	3
#define FLASH_DRIVE_INIT_ERR_LIBUSB_RESET_DRIVER			4
#define	FLASH_DRIVE_INIT_ERR_LIBUSB_CLAIM_INTERFACE			5
#define FLASH_DRIVE_INIT_ERR_SCSI_INQUIRY					6
#define FLASH_DRIVE_INIT_ERR_TEST_UNIT_READY				7
#define FLASH_DRIVE_INIT_ERR_READ_CAPACITY					8
#define FLASH_DRIVE_INIT_ERR_BLOCKSIZE_NOT_512				9
#define FLASH_DRIVE_INIT_ERR_LIBUSB_KERNEL_ACTIVE			10

// flash_deinit() Error Return Values
#define FLASH_DRIVE_DEINIT_SUCCESS							0
#define FLASH_DRIVE_DEINIT_ERR_REATTACH_KERNEL_DRIVER		1

// Error returned by flash_drive_send_data(). It actually returns errors codes which are returned by libusb functions, but positive number.
#define FLASH_DRIVE_SEND_ERR_SUCCESS						0 	/** Success */
#define FLASH_DRIVE_SEND_ERR_IO								1 	/** Input/output error */
#define FLASH_DRIVE_SEND_ERR_INVALID_PARAM					2	/** Invalid parameter */
#define FLASH_DRIVE_SEND_ERR_ACCESS							3	/** Access denied (insufficient permissions) */
#define FLASH_DRIVE_SEND_ERR_NO_DEVICE						4	/** No such device (it may have been disconnected) */
#define FLASH_DRIVE_SEND_ERR_NOT_FOUND						5	/** Entity not found */
#define FLASH_DRIVE_SEND_ERR_BUSY							6		/** Resource busy */
#define FLASH_DRIVE_SEND_ERR_TIMEOUT						7	/** Operation timed out */
#define FLASH_DRIVE_SEND_ERR_OVERFLOW						8	/** Overflow */
#define FLASH_DRIVE_SEND_ERR_PIPE							9	/** Pipe error */
#define FLASH_DRIVE_SEND_ERR_INTERRUPTED					10	/** System call interrupted (perhaps due to signal) */
#define FLASH_DRIVE_SEND_ERR_NO_MEM							11	/** Insufficient memory */
#define FLASH_DRIVE_SEND_ERR_NOT_SUPPORTED					12	/** Operation not supported or unimplemented on this platform */
#define FLASH_DRIVE_SEND_ERR_OTHER							99	/** Other error */

// Error returned by flash_drive_receive_data(). It actually returns errors which are returned by libusb functions, but positive number.
#define FLASH_DRIVE_RECEIVE_ERR_SUCCESS							0 	/** Success */
#define FLASH_DRIVE_RECEIVE_ERR_IO								1 	/** Input/output error */
#define FLASH_DRIVE_RECEIVE_ERR_INVALID_PARAM					2	/** Invalid parameter */
#define FLASH_DRIVE_RECEIVE_ERR_ACCESS							3	/** Access denied (insufficient permissions) */
#define FLASH_DRIVE_RECEIVE_ERR_NO_DEVICE						4	/** No such device (it may have been disconnected) */
#define FLASH_DRIVE_RECEIVE_ERR_NOT_FOUND						5	/** Entity not found */
#define FLASH_DRIVE_RECEIVE_ERR_BUSY							6		/** Resource busy */
#define FLASH_DRIVE_RECEIVE_ERR_TIMEOUT							7	/** Operation timed out */
#define FLASH_DRIVE_RECEIVE_ERR_OVERFLOW						8	/** Overflow */
#define FLASH_DRIVE_RECEIVE_ERR_PIPE							9	/** Pipe error */
#define FLASH_DRIVE_RECEIVE_ERR_INTERRUPTED						10	/** System call interrupted (perhaps due to signal) */
#define FLASH_DRIVE_RECEIVE_ERR_NO_MEM							11	/** Insufficient memory */
#define FLASH_DRIVE_RECEIVE_ERR_NOT_SUPPORTED					12	/** Operation not supported or unimplemented on this platform */
#define FLASH_DRIVE_RECEIVE_ERR_OTHER							99	/** Other error */

// DEFAULT values for Descriptor of USB flash drive (For BULK ONLY USB TRANSFER)
#define BULK_ONLY_DEFAULT_CONFIGURATION_VALUE	 			1
#define BULK_ONLY_DEFAULT_ENDPOINT_IN 						0x81
#define BULK_ONLY_DEFAULT_ENDPOINT_OUT 						0x02
#define BULK_ONLY_DEFAULT_LUN_NUMBER						0
#define BULK_ONLY_DEFAULT_INTERFACE_NUMBER					0

// libusb Timeout for Control and Bulk Transfers. Timeouts are in milliseconds.
#define OUT_TIMEOUT											1000
#define IN_TIMEOUT											1000
#define CONTROL_TIMEOUT										1000

// Macros for Reset using a control transfer
// as per USB Mass Storage Bulk Only Specification 1.0
#define RESET_CONTROL_REQUEST_TYPE		0x21		//Class, Interface and Device to Host
#define RESET_CONTROL_REQUEST 			0xFF
#define RESET_CONTROL_VALUE				0x00
#define RESET_CONTROL_LENGTH 			0x01
#define RESET_RESPONSE_DATA_LENGTH		0x00

// Macros for getting MaxLUN using a control transfer
// as per USB Mass Storage Bulk Only Specification 1.0
#define GETMAXLUN_CONTROL_REQUEST_TYPE		0xA1		//Class, Interface and Device to Host
#define GETMAXLUN_CONTROL_REQUEST 			0xFE
#define GETMAXLUN_CONTROL_VALUE				0x00
#define GETMAXLUN_CONTROL_LENGTH 			0x01
#define GETMAXLUN_RESPONSE_DATA_LENGTH		0x01

uint8_t flash_drive_init(void);
uint8_t flash_drive_deinit(void);
uint8_t flash_drive_send_data(unsigned char *data_ptr, int no_of_bytes);
uint8_t flash_drive_receive_data(unsigned char *data_ptr, int no_of_bytes, int *no_of_actually_transferred_bytes);
uint8_t flash_drive_reset();
uint8_t flash_drive_reset_manual(void);
uint8_t flash_drive_GetMaxLUN(uint8_t *MaxLUN);

#endif