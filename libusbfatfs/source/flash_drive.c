/***************************************************************************************************
 *
 *  Copyright 2014  Rounak SIngh (rounaksingh17@gmail.com)
 *
 *  Permission to use, copy, modify, and distribute this software
 *  and its documentation for any purpose and without fee is hereby
 *  granted, provided that the above copyright notice appear in all
 *  copies and that both that the copyright notice and this
 *  permission notice and warranty disclaimer appear in supporting
 *  documentation, and that the name of the author not be used in
 *  advertising or publicity pertaining to distribution of the
 *  software without specific, written prior permission.
 *
 *  The author disclaim all warranties with regard to this
 *  software, including all implied warranties of merchantability
 *  and fitness.  In no event shall the author be liable for any
 *  special, indirect or consequential damages or any damages
 *  whatsoever resulting from loss of use, data or profits, whether
 *  in an action of contract, negligence or other tortious action,
 *  arising out of or in connection with the use or performance of
 *  this software.
 *
 ****************************************************************************************************/
#include <ps4.h>
#include "flash_drive.h"
#include "MassStoreCommands.h"
#include "print_struct.h"
#include "config.h"

#define printf(...) (void)0

// The pointer to libusb device handle. This variable is internal to flash_drive.c
static struct libusb_device_handle *devh = NULL;
// variable to save the info about the attached kernel driver to interface of the device.
//static uint8_t active_kernel_driver=0;

/**
 * Initializes the USB Flash drive. Function finds the USB flash device using its VID and PID, then
 * initiate a sequence of functions to enumerate the device and start the communication with it.
 *
 * @return  Errors/Success (For more info on Errors, Please refer the flash_drive.h file)
 */
uint8_t flash_drive_init(void)
{
	int ret_val;
	SCSI_Inquiry_Response_t req_sense_res;
	SCSI_Capacity_t capacity;

	// libusb initialization
	ret_val = sceUsbdInit();
	if (ret_val < 0)
		return FLASH_DRIVE_INIT_ERR_LIBUSB_INIT;

	// Find the USB device and open it for communication.
	devh = sceUsbdOpenDeviceWithVidPid(USB_DEVICE_VID, USB_DEVICE_PID);
	if(devh == 0)
	{
		printf("\nCouldnot Open the device with VID= %04X and PID= %04X.\n",USB_DEVICE_VID, USB_DEVICE_PID);
		return FLASH_DRIVE_INIT_ERR_LIBUSB_OPEN_DEVICE;
	}

	printf("\nDevice opened with VID= %04X and PID= %04X.",USB_DEVICE_VID, USB_DEVICE_PID);


	// check for the kernel driver, if attached at the interface 0
	// If attached then dettach and set the active_kernel_driver variable
	// to notify the reattach process at the end of the program.
	/* ret_val=libusb_kernel_driver_active(devh, BULK_ONLY_DEFAULT_INTERFACE_NUMBER);

	// Return Value: (As per the libusb 1.0 manual)
	// 0 : No Kernel Driver active.
	// 1 : Kernel Driver Active.
	// else : libusb_kernel_driver_active Error
	if(ret_val == 0)
	{
		printf("\nKernel Driver not active on the Interface.\n");
		active_kernel_driver=0;
	}
	else if(ret_val == 1)
	{
		printf("\nKernel Driver active on the Interface.\nExecuting procedure to dettach Kernel driver...\n");

		// Important to remove the USB flash drive from the kernel access
		ret_val=libusb_detach_kernel_driver(devh, BULK_ONLY_DEFAULT_INTERFACE_NUMBER);
		if(ret_val<0)
		{
			printf("\nCouldnot detach the device.\n");
			return FLASH_DRIVE_INIT_ERR_LIBUSB_DETTACH_KERNEL_DRIVER;
		}
		else
		{
			active_kernel_driver=1;
			printf("\nDevice detached.\n");
		}

	}
	else
	{
		printf("\nCould not check the kernel_driver_active Error: %d.\n", ret_val);
		active_kernel_driver=0;
		return FLASH_DRIVE_INIT_ERR_LIBUSB_KERNEL_ACTIVE;
	}*/


/*  // As per suggestion from Alan Stern from libusb mailing list
	// To see the suugestion
	// http://libusb.6.n5.nabble.com/libusb-bulk-transfer-error-9-Pipe-Halt-at-EndPoint-0x81-while-receiving-data-from-usb-flash-drive-tt5712940.html#a5712943
	// configuration set to B_CONFIGURATION_VALUE
	ret_val=libusb_set_configuration(devh,B_CONFIGURATION_VALUE);
	if(ret_val<0)
	{
		printf("\nConfiguration failed.\n");
		return ret_val;
	}
	else
	{
		printf("\nConfigured.\n");
	}*/

	// Reset the USB device (using USB Control transfer) to receive the SCSI commands.
	// Though as per USB Mass Storage specification (Bulk Only),
	// a reset (control transfer) request should be sent to USB flash drive as first command,
	// But the libusb provides us a reset functions so we are going to use it.
	ret_val=flash_drive_reset();
	if(ret_val<0)
	{
		printf("\nCouldnot reset device. Error: %d\n",ret_val);
		return FLASH_DRIVE_INIT_ERR_LIBUSB_RESET_DRIVER;
	}

	printf("\nReset device completed.\n");

	// claim the interface 0 for data transfer (Interface Zero is normally the )
	/*ret_val=libusb_claim_interface(devh, BULK_ONLY_DEFAULT_INTERFACE_NUMBER);
	if(ret_val<0)
	{
		printf("\nCouldnot claim the interface.\n");
		return FLASH_DRIVE_INIT_ERR_LIBUSB_CLAIM_INTERFACE;
	}*/

	printf("\nInterface claimed.\n");

	// send Inquiry SCSI command
	ret_val = MassStore_Inquiry( BULK_ONLY_DEFAULT_LUN_NUMBER, &req_sense_res );
	if(ret_val > 0)
	{
		printf("MassStore_Inquiry Fail\n");
		return FLASH_DRIVE_INIT_ERR_SCSI_INQUIRY;
	}
	else
	{
		printf("MassStore_Inquiry successful.\n");

		// Print the struct Inquiry using a special print function in print_struct.c
		print_struct_INQ(&req_sense_res);

		// Test UNIT Ready
		ret_val=MassStore_TestUnitReady(BULK_ONLY_DEFAULT_LUN_NUMBER);
		if(ret_val > 0)
		{
			printf("Mass Storage Not Ready. Do a Request Sense.\n");
			return FLASH_DRIVE_INIT_ERR_TEST_UNIT_READY;
		}

		printf("Mass Storage Ready.\n");

		printf("\nReading Capacity of Mass Storage device (LUN=0):\n");

		// Read the Block Size and no of Blocks in USB flash drive
		// Since, the Petit FATfs is compatible with Block Size 512 bytes
		// therefore, it is important to check the Block Size.
		ret_val=MassStore_ReadCapacity( BULK_ONLY_DEFAULT_LUN_NUMBER, &capacity);
		if(ret_val > 0)
		{
			printf("Mass Storage Read Capacity failed.\n");
			return FLASH_DRIVE_INIT_ERR_READ_CAPACITY;
		}
		else
		{
			printf("\nMass Storage Read Capacity success.\n");
			printf("No of Blocks: 0x%04X, %d\n",capacity.Blocks, capacity.Blocks);
			printf("BlockSize: 0x%04X, %d bytes\n",capacity.BlockSize, capacity.BlockSize);
		}
		// Check for Block Size of flash in USB flash drive.
		// If Block Size is not 512 bytes then return Error.
		if(capacity.BlockSize!=512)
		{
			printf("Error: libPFF is not compatiable with Block Size not equal to 512 bytes");
			return FLASH_DRIVE_INIT_ERR_BLOCKSIZE_NOT_512;
		}
	}

	return FLASH_DRIVE_INIT_SUCCESS;
}/* End flash_drive_init() */
/*****************************************************************************************************************/

/**
 * Deinitializes the USB flash_drive after use.
 *
 * @return Errors/Success (For more info on Errors, Please refer the flash_drive.h file)
 */
uint8_t flash_drive_deinit()
{
	int ret_val = FLASH_DRIVE_DEINIT_SUCCESS;

	// release the interface zero if claimed.
	/*ret_val=libusb_release_interface(devh, BULK_ONLY_DEFAULT_INTERFACE_NUMBER);
	if(ret_val<0)
	{
		printf("\nRelease of Interface Failed.	%d\n",ret_val);
		//BEWARE donot add return here because below functions are important for execution.
	}
	else
		printf("\nInterfaced released.\n");*/

	// Reattach the kernel driver
	/*if(active_kernel_driver==1)
	{
		ret_val=libusb_attach_kernel_driver(devh, BULK_ONLY_DEFAULT_INTERFACE_NUMBER);
		if(ret_val<0)
		{
			printf("\nReattach Failed.	%d\n",ret_val);
			// BEWARE: We are not returning here because at the bottom ret_val is not going to change
			//  therefore it will automatically return.
			// return USB_ATTACH_KERNEL_DRIVER_ERROR;
			ret_val = FLASH_DRIVE_DEINIT_ERR_REATTACH_KERNEL_DRIVER;
		}
		else
			printf("\nReattach Completed.\n");
	}*/

	// please donot put the libusb_close() and libusb_exit() above libusb_attach_kernel_driver()
	// as libusb_attach_kernel_driver() requires the devh handler to attach the kernel driver
	// to provided interface.
	if(devh)
		sceUsbdClose(devh);

	sceUsbdExit();

	return (uint8_t) ret_val;
}/* End flash_drive_deinit() */
/*****************************************************************************************************************/

/**
 * Sends the data to the USB flash device using USB Bulk transfer.
 * The function uses libusb_bulk_transfer() as transport function.
 * In addition the function implement just Bulk-Only transfer for
 * default Endpoint OUT (that is 0x02).
 *
 * @param  data_ptr    Pointer to data buffer
 * @param  no_of_bytes Number of bytes in data buffer
 * @return             Errors/Success (Refer the flash_drive.h for more information.)
 */
uint8_t flash_drive_send_data(unsigned char *data_ptr, int no_of_bytes)
{
	int ret_val;
	int actual_length=123;	//garbage value

	// transfer the data to ENDPOINT OUT using USB Bulk Transfer.
	ret_val=sceUsbdBulkTransfer(devh, BULK_ONLY_DEFAULT_ENDPOINT_OUT, data_ptr, no_of_bytes, &actual_length, OUT_TIMEOUT);

	// The if condition checks whether ret_val equals 0 (libusb SUCCESS) and
	// Number of bytes to send is equal to the number of bytes which are actually sent.
	if((ret_val == 0) && (actual_length == no_of_bytes))
	{
		#ifdef DEBUG_FLASH_DRIVE
		printf("\nSend Complete. %d  %d\n",no_of_bytes, actual_length);
		#endif
	}
	else
	{
		#ifdef DEBUG_FLASH_DRIVE
		printf("\nSend Failure.	%d\n",ret_val);
		#endif
	}

	// if success then returns zero.
	return (uint8_t)(-ret_val);
}/* End flash_drive_send_data() */
/*****************************************************************************************************************/

/**
 * Receive the data send by the USB device. But the data transfer is initiated by USB Host.
 * The function implement Bulk Only transfer with default IN enpoint which is 0x81.
 * The function returns does not return error if actually received bytes are less than
 * the desired number of bytes.
 *
 * @param  data_ptr                      Pointer to data buffer
 * @param  no_of_bytes                   Number of bytes to receive  or Length of data buffer
 * @param  no_of_actually_received_bytes Number of Actually received bytes
 * @return                               Error code or Success
 */
uint8_t flash_drive_receive_data(unsigned char *data_ptr, int no_of_bytes, int *no_of_actually_received_bytes)
{
	int ret_val;

	ret_val=sceUsbdBulkTransfer(devh, BULK_ONLY_DEFAULT_ENDPOINT_IN, data_ptr, no_of_bytes, no_of_actually_received_bytes, IN_TIMEOUT);
	if(ret_val==-9)
	{
		#ifdef DEBUG_FLASH_DRIVE
		printf("PIPE HALT or PIPE STALL. LIBUSB ERROR: -9\n");
		#endif
	}
	else if(ret_val<0)
	{
		#ifdef DEBUG_FLASH_DRIVE
		printf("\nReceive Failure. LIBUSB ERROR: %d",ret_val);
		#endif
	}
	else
	{
		#ifdef DEBUG_FLASH_DRIVE
		printf("\nReceive Complete. \tReceivedBytes:%d\tActuallyReceivedBytes:%d\n",no_of_bytes,*no_of_actually_received_bytes);
		#endif
	}

	return (uint8_t)(-ret_val);
} /* End flash_drive_receive_data() */
/*****************************************************************************************************************/

/**
 * Resets the USB flash drive using a default provided by the libusb library.
 * The Function does a soft reset to flash drive to make it receive a new Command Block Wrapper.
 *
 * @return Error codes/Success
 */
uint8_t flash_drive_reset()
{
	int ret_val;
	//reset the device
	ret_val=sceUsbdResetDevice(devh);

	return (uint8_t)(-ret_val);
}/* End flash_drive_reset() */
/*****************************************************************************************************************/

/**
 * Resets the flash drive maually using a USB control transfer.
 *
 * @return  Error codes/Success
 */
uint8_t flash_drive_reset_manual(void)
{
	int ErrorCode;
	// For getting maximum LUN, we need to send a control transfer as per USB Mass Storage Bulk Only Specification 1.0 section 3.2
	// Taking Interface (USB) = Index (libusb_control_transfer) = 0
	ErrorCode=sceUsbdControlTransfer(devh,	RESET_CONTROL_REQUEST_TYPE, RESET_CONTROL_REQUEST, RESET_CONTROL_VALUE, 0,
	0, RESET_CONTROL_LENGTH, CONTROL_TIMEOUT);

	// if return value is less than zero, then Error.
	// if zero then successful.
	if(ErrorCode < 0)
	{
		#ifdef DEBUG_FLASH_DRIVE
		printf("\nReset Error.\n");
		#endif
	}

	return (uint8_t)(-ErrorCode);

}/* End flash_drive_reset_manual() */
/*****************************************************************************************************************/

/**
 * Gets the Maximum Logical Unit Number(LUN) from USB flash drive.
 * As per USB Mass Storage Bulk only specification, USB host should do a control transfer
 * if it needs to know the LUN of flash drive, there is no SCSI command.
 *
 * About the Maximum LUN returned by USB flash drive:
 *
 * 1> if MaxLUN is zero, then LUN not supported.
 *
 * 2> if MaxLUN is one then the LUN in flash drive is zero
 *
 * 3> if MaxLUN is n then the LUN in flash drive is from zero to (n-1). (Maximum value of n is 0x10 or 16)
 *
 * @param  MaxLUN 	Data pointer where to store the MAXLUN after receiving it by control transfer. (LUN numbers should be from 0 to 15 as per USB Mass Storage specification)
 * @return        	Error codes/Success
 */
uint8_t flash_drive_GetMaxLUN(uint8_t *MaxLUN)
{
	int ErrorCode;
	// For getting maximum LUN, we need to send a control transfer as per USB Mass Storage Bulk Only Specification 1.0 section 3.2
	// Taking Interface (USB) = Index (libusb_control_transfer) = 0
	ErrorCode=sceUsbdControlTransfer(devh,	GETMAXLUN_CONTROL_REQUEST_TYPE, GETMAXLUN_CONTROL_REQUEST, GETMAXLUN_CONTROL_VALUE, 0,
	MaxLUN, GETMAXLUN_CONTROL_LENGTH, CONTROL_TIMEOUT);

	// if libusb_control_transfer returns 0 means successful
	// and if MaxLUNIndex is zero means media controller has STALL (as per USB Mass Storage Bulk only specification)
	// therefore clear STALL or Endpoint halt
	if (ErrorCode == 0 && *MaxLUN==0)
	{
		// Clear the pipe stall
		// here endpoint is zero (0) that is Control Endpoint
		ErrorCode = sceUsbdClearHalt(devh, 0);
	}

	return (uint8_t)(-ErrorCode);

}/* End flash_drive_GetMaxLUN() */
/****************************************************************************************************************/
