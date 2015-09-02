/*
             LUFA Library
     Copyright (C) Dean Camera, 2009.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2009  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, and distribute this software
  and its documentation for any purpose and without fee is hereby
  granted, provided that the above copyright notice appear in all
  copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Mass Storage Device commands, to issue MSD commands to the device for
 *  reading device status, capacity, and other characteristics. This file
 *  also contains block read and write functions, so that device blocks
 *  can be read and written. In general, these functions would be chained
 *  to a FAT library to give file-level access to an attached device's contents.
 *
 *  \note Many Mass Storage devices on the market are non-compliant to the
 *        specifications and thus can prove difficult to interface with. It
 *        may be necessary to retry the functions in the module several times
 *        after they have returned and error to successfully send the command
 *        to the device. Some devices may also need to have the stream function
 *        timeout period extended beyond 100ms (some badly designed devices exceeding
 *        1.5 seconds occasionally) by defining USB_STREAM_TIMEOUT_MS to a
 *        larger value in the project makefile and passing it to the compiler
 *        via the -D switch.
 */
 
#define  INCLUDE_FROM_MASSSTORE_COMMANDS_C
#include "MassStoreCommands.h"
#include "flash_drive.h"
#include "print_struct.h"
#include "common.h"

/** Current Tag value used in issued CBWs to the device. This is automatically incremented
 *  each time a command is sent, and is not externally accessible.
 */
static uint32_t MassStore_Tag = 1;


/** Routine to send the current CBW to the device, and increment the Tag value as needed.
 *
 *  \param[in] SCSICommandBlock  Pointer to a SCSI command block structure to send to the attached device
 *  \param[in,out] BufferPtr     Pointer to a buffer for the data to send or receive to/from the device, or NULL if no data
 *
 *  \return A value from the Pipe_Stream_RW_ErrorCodes_t enum
 */
static uint8_t MassStore_SendCommand(CommandBlockWrapper_t* SCSICommandBlock, void* BufferPtr)
{
	uint8_t ErrorCode;

	/* Each transmission should have a unique tag value, increment before use */
	SCSICommandBlock->Tag = ++MassStore_Tag;

	/* Wrap Tag value when invalid - MS class defines tag values of 0 and 0xFFFFFFFF to be invalid */
	if (MassStore_Tag == 0xFFFFFFFF)
	  MassStore_Tag = 1;

	#ifdef DEBUG_MASS_STORE_COMMANDS
	printf("Size of CommandBlockWrapper_t= %d\n",sizeof(CommandBlockWrapper_t));
	#endif

	ErrorCode = flash_drive_send_data((unsigned char *)SCSICommandBlock, sizeof(CommandBlockWrapper_t));
	if(ErrorCode > 0)
		return ErrorCode;

	// Send data if any
	if ((BufferPtr != NULL) && ((ErrorCode = MassStore_SendReceiveData(SCSICommandBlock, BufferPtr)) != 0))
	{
		return ErrorCode;
	}
		
	return 0;
} /* End MassStore_SendCommand() */
/*****************************************************************************************************************/

/** Sends or receives the transaction's data stage to or from the attached device, reading or
 *  writing to the nominated buffer.
 *
 *  \param[in] SCSICommandBlock  Pointer to a SCSI command block structure being sent to the attached device
 *  \param[in,out]  BufferPtr    Pointer to the data buffer to read from or write to
 *
 *  \return A value from the Pipe_Stream_RW_ErrorCodes_t enum
 */
static uint8_t MassStore_SendReceiveData(CommandBlockWrapper_t* SCSICommandBlock, void* BufferPtr)
{
	uint8_t  ErrorCode;
	uint16_t BytesRem  = SCSICommandBlock->DataTransferLength;
	int actual_no_received;

	// Check that direction of the SCSI command data stage
	// And take appropriate action as per the direction.
	if (SCSICommandBlock->Flags & COMMAND_DIRECTION_DATA_IN)
	{
		ErrorCode = flash_drive_receive_data(BufferPtr, BytesRem, &actual_no_received);
		if(ErrorCode > 0)
			return ErrorCode;		// return Errorcode 

		// Here we know that the received number of bytes must be BytesRem. Since it is sent in the CBW.
		if(BytesRem != actual_no_received)
			return 120;				// Error 120 --- ERROR CODE: actual_no_received not equal to desired BytesRem
	}
	else
	{
		ErrorCode = flash_drive_send_data(BufferPtr, BytesRem);
		if(ErrorCode > 0)
			return ErrorCode;
	}
	
	return 0;
} /* End MassStore_SendReceiveData() */
/*****************************************************************************************************************/

/** Routine to receive the current CSW from the device.
 *
 *  \param[out] SCSICommandStatus  Pointer to a destination where the returned status data should be stored
 *
 *  \return A value from the Pipe_Stream_RW_ErrorCodes_t enum, or MASS_STORE_SCSI_COMMAND_FAILED if the SCSI command fails
 */
static uint8_t MassStore_GetReturnedStatus(CommandStatusWrapper_t* SCSICommandStatus)
{
	uint8_t ErrorCode;
	int actual_no_received;

	ErrorCode = flash_drive_receive_data((unsigned char *)SCSICommandStatus, sizeof(CommandStatusWrapper_t) , &actual_no_received);
	// Success
	if((ErrorCode==0) && (actual_no_received==sizeof(CommandStatusWrapper_t)))
	{
		// Check to see if command failed
		if (SCSICommandStatus->Status != Command_Pass)
			ErrorCode = MASS_STORE_SCSI_COMMAND_FAILED;
	}
	
	// on success it returns 0 
	// on error returns error code
	return ErrorCode;
} /* End MassStore_GetReturnedStatus() */
/*****************************************************************************************************************/

/***************************************************************************/
/*For Reset and GetMaxLUN function we are directly implementing libusb built-in function.*/
/***************************************************************************/
/** Issues a Mass Storage class specific request to reset the attached device's Mass Storage interface,
 *  readying the device for the next CBW.
 *
 *  \return A value from the USB_Host_SendControlErrorCodes_t enum, or MASS_STORE_SCSI_COMMAND_FAILED if the SCSI command fails
 */
/*uint8_t MassStore_MassStorageReset(void)
{
	USB_ControlRequest = (USB_Request_Header_t)
		{
			.bmRequestType = (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE),
			.bRequest      = REQ_MassStorageReset,
			.wValue        = 0,
			.wIndex        = 0,
			.wLength       = 0,
		};
	
	Select the control pipe for the request transfer
	Pipe_SelectPipe(PIPE_CONTROLPIPE);

	return USB_Host_SendControlRequest(NULL);
}
*/

/** Issues a Mass Storage class specific request to determine the index of the highest numbered Logical
 *  Unit in the attached device.
 *
 *  \note Some devices do not support this request, and will STALL it when issued. To get around this,
 *        on unsupported devices the max LUN index will be reported as zero and no error will be returned
 *        if the device STALLs the request.
 *
 *  \param[out] MaxLUNIndex  Pointer to the location that the maximum LUN index value should be stored
 *
 *  \return A value from the USB_Host_SendControlErrorCodes_t enum, or MASS_STORE_SCSI_COMMAND_FAILED if the SCSI command fails
 */

/*uint8_t MassStore_GetMaxLUN(uint8_t* const MaxLUNIndex)
{
	uint8_t ErrorCode ;

	USB_ControlRequest = (USB_Request_Header_t)
		{
			.bmRequestType = (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE),
			.bRequest      = REQ_GetMaxLUN,
			.wValue        = 0,
			.wIndex        = 0,
			.wLength       = 1,
		};
		
	// Select the control pipe for the request transfer
	Pipe_SelectPipe(PIPE_CONTROLPIPE);

	if ((ErrorCode = USB_Host_SendControlRequest(MaxLUNIndex)) == HOST_SENDCONTROL_SetupStalled)
	{
		// Clear the pipe stall 
		Pipe_ClearStall();
	
		// Some faulty Mass Storage devices don't implement the GET_MAX_LUN request, so assume a single LUN
		*MaxLUNIndex = 0;
		
		// Clear the error, and pretend the request executed correctly if the device STALLed it
		ErrorCode = HOST_SENDCONTROL_Successful;
	}

	return ErrorCode;
}*/
/************************************************************************************************************************/

/** Issues a SCSI Inquiry command to the attached device, to determine the device's information. This
 *  gives information on the device's capabilities.
 *
 *  \param[in] LUNIndex    Index of the LUN inside the device the command is being addressed to
 *  \param[out] InquiryPtr  Pointer to the inquiry data structure where the inquiry data from the device is to be stored
 *
 *  \return A value from the Pipe_Stream_RW_ErrorCodes_t enum, or MASS_STORE_SCSI_COMMAND_FAILED if the SCSI command fails
 */
uint8_t MassStore_Inquiry(const uint8_t LUNIndex, SCSI_Inquiry_Response_t* const InquiryPtr)
{
	uint8_t ErrorCode;

	// Create a CBW with a SCSI command to issue INQUIRY command
	CommandBlockWrapper_t SCSICommandBlock = (CommandBlockWrapper_t)
		{
			.Signature          = CBW_SIGNATURE,
			.DataTransferLength = sizeof(SCSI_Inquiry_Response_t),
			.Flags              = COMMAND_DIRECTION_DATA_IN,
			.LUN                = LUNIndex,
			.SCSICommandLength  = 6,
			.SCSICommandData    =
				{
					SCSI_CMD_INQUIRY,
					0x00,                   			// Reserved
					0x00,                   			// Reserved
					0x00,                   			// Reserved
					sizeof(SCSI_Inquiry_Response_t), 	// Allocation Length
					0x00                    			// Unused (control)
				}
		};

	// Print the Command Block Wrapper for Inquiry
	print_struct_CBW(&SCSICommandBlock);

	CommandStatusWrapper_t SCSICommandStatus;

	// Send the command and any data to the attached device.
	if ((ErrorCode = MassStore_SendCommand(&SCSICommandBlock, InquiryPtr)) != 0)
	{
		return ErrorCode;
	}
	
	// Retrieve status information from the attached device
	if ((ErrorCode = MassStore_GetReturnedStatus(&SCSICommandStatus)) != 0)
	{
		return ErrorCode;
	}

	// Print the Command Status for Inquiry
	print_struct_CSW(&SCSICommandStatus);

	// on success it returns 0 
	// on error returns error code
	return ErrorCode;
} /* End MassStore_Inquiry() */
/************************************************************************************************************************/

/** Issues a SCSI Request Sense command to the attached device, to determine the current SCSI sense information. This
 *  gives error codes for the last issued SCSI command to the device.
 *
 *  \param[in] LUNIndex   Index of the LUN inside the device the command is being addressed to
 *  \param[out] SensePtr  Pointer to the sense data structure where the sense data from the device is to be stored
 *
 *  \return 
 */
uint8_t MassStore_RequestSense(const uint8_t LUNIndex, SCSI_Request_Sense_Response_t* const SensePtr)
{
	uint8_t ErrorCode ;

	// Create a CBW with a SCSI command to issue REQUEST SENSE command.
	CommandBlockWrapper_t SCSICommandBlock = (CommandBlockWrapper_t)
		{
			.Signature          = CBW_SIGNATURE,
			.DataTransferLength = sizeof(SCSI_Request_Sense_Response_t),
			.Flags              = COMMAND_DIRECTION_DATA_IN,
			.LUN                = LUNIndex,
			.SCSICommandLength  = 6,
			.SCSICommandData =
				{
					SCSI_CMD_REQUEST_SENSE,
					0x00,                   				// Reserved
					0x00,                   				// Reserved
					0x00,                   				// Reserved
					sizeof(SCSI_Request_Sense_Response_t), 	// Allocation Length
					0x00                    				// Unused (control)
				}
		};
	
	CommandStatusWrapper_t SCSICommandStatus;

	// Send the command and any data to the attached device
	if ((ErrorCode = MassStore_SendCommand(&SCSICommandBlock, SensePtr)) != 0)
	{
		return ErrorCode;
	}
	
	// Retrieve status information from the attached device
	if ((ErrorCode = MassStore_GetReturnedStatus(&SCSICommandStatus)) != 0)
	{
		return ErrorCode;
	}

	// on success it returns 0 
	// on error returns error code
	return ErrorCode;
} /* End MassStore_RequestSense() */
/************************************************************************************************************************/

/** Issues a SCSI Device Block Read command to the attached device, to read in one or more data blocks from the
 *  storage medium into a buffer.
 *
 *  \param[in] LUNIndex      Index of the LUN inside the device the command is being addressed to
 *  \param[in] BlockAddress  Start block address to read from
 *  \param[in] Blocks        Number of blocks to read from the device (function can read only 256 sectors at a time)
 *  \param[in] BlockSize     Size in bytes of each block to read
 *  \param[out] BufferPtr    Pointer to the buffer where the read data is to be written to
 *
 *  \return A value from the Pipe_Stream_RW_ErrorCodes_t enum, or MASS_STORE_SCSI_COMMAND_FAILED if the SCSI command fails
 */
uint8_t MassStore_ReadDeviceBlock(const uint8_t LUNIndex, const uint32_t BlockAddress,
                                  const uint8_t Blocks, const uint16_t BlockSize, void* BufferPtr)
{
	uint8_t ErrorCode;

	// Create a CBW with a SCSI command to read in the given blocks from the device
	CommandBlockWrapper_t SCSICommandBlock = (CommandBlockWrapper_t)
		{
			.Signature          = CBW_SIGNATURE,
			.DataTransferLength = ((uint32_t)Blocks * BlockSize),
			.Flags              = COMMAND_DIRECTION_DATA_IN,
			.LUN                = LUNIndex,
			.SCSICommandLength  = 10,
			.SCSICommandData    =
				{
					SCSI_CMD_READ_10,
					0x00,                   // Unused (control bits, all off)
					(BlockAddress >> 24),   // MSB of Block Address
					(BlockAddress >> 16),
					(BlockAddress >> 8),
					(BlockAddress & 0xFF),  // LSB of Block Address
					0x00,                   // Unused (reserved)
					0x00,                   // MSB of Total Blocks to Read
					Blocks,                 // LSB of Total Blocks to Read
					0x00                    // Unused (control)
				}
		};
	
	CommandStatusWrapper_t SCSICommandStatus;

	// Send the command and any data to the attached device
	if ((ErrorCode = MassStore_SendCommand(&SCSICommandBlock, BufferPtr)) != 0)
	{
		return ErrorCode;
	}
	
	// Retrieve status information from the attached device
	if ((ErrorCode = MassStore_GetReturnedStatus(&SCSICommandStatus)) != 0)
	{
		return ErrorCode;
	}

	return ErrorCode;
} /* End MassStore_ReadDeviceBlock() */
/************************************************************************************************************************/

/** Issues a SCSI Device Block Write command to the attached device, to write one or more data blocks to the
 *  storage medium from a buffer.
 *
 *  \param[in] LUNIndex      Index of the LUN inside the device the command is being addressed to
 *  \param[in] BlockAddress  Start block address to write to
 *  \param[in] Blocks        Number of blocks to write to in the device
 *  \param[in] BlockSize     Size in bytes of each block to write
 *  \param[in] BufferPtr     Pointer to the buffer where the write data is to be sourced from
 *
 *  \return A value from the Pipe_Stream_RW_ErrorCodes_t enum, or MASS_STORE_SCSI_COMMAND_FAILED if the SCSI command fails
 */
uint8_t MassStore_WriteDeviceBlock(const uint8_t LUNIndex, const uint32_t BlockAddress,
                                   const uint8_t Blocks, const uint16_t BlockSize, void* BufferPtr)
{
	uint8_t ErrorCode ;

	// Create a CBW with a SCSI command to write the given blocks to the device
	CommandBlockWrapper_t SCSICommandBlock = (CommandBlockWrapper_t)
		{
			.Signature          = CBW_SIGNATURE,
			.DataTransferLength = ((uint32_t)Blocks * BlockSize),
			.Flags              = COMMAND_DIRECTION_DATA_OUT,
			.LUN                = LUNIndex,
			.SCSICommandLength  = 10,
			.SCSICommandData    =
				{
					SCSI_CMD_WRITE_10,
					0x00,                   // Unused (control bits, all off)
					(BlockAddress >> 24),   // MSB of Block Address
					(BlockAddress >> 16),
					(BlockAddress >> 8),
					(BlockAddress & 0xFF),  // LSB of Block Address
					0x00,                   // Unused (reserved)
					0x00,                   // MSB of Total Blocks to Write
					Blocks,                 // LSB of Total Blocks to Write
					0x00                    // Unused (control)
				}
		};
	
	CommandStatusWrapper_t SCSICommandStatus;

	// Send the command and any data to the attached device 
	if ((ErrorCode = MassStore_SendCommand(&SCSICommandBlock, BufferPtr)) != 0)
	{
		return ErrorCode;
	}
	
	// Retrieve status information from the attached device
	if ((ErrorCode = MassStore_GetReturnedStatus(&SCSICommandStatus)) != 0)
	{
		return ErrorCode;
	}

	return ErrorCode;
} /* End MassStore_WriteDeviceBlock() */
/************************************************************************************************************************/

/** Issues a SCSI Device Test Unit Ready command to the attached device, to determine if the device is ready to accept
 *  other commands.
 *
 *  \param[in] LUNIndex      Index of the LUN inside the device the command is being addressed to
 *
 *  \return A value from the Pipe_Stream_RW_ErrorCodes_t enum, or MASS_STORE_SCSI_COMMAND_FAILED if the SCSI command fails
 */
uint8_t MassStore_TestUnitReady(const uint8_t LUNIndex)
{
	uint8_t ErrorCode;	

	/* Create a CBW with a SCSI command to issue TEST UNIT READY command */
	CommandBlockWrapper_t SCSICommandBlock = (CommandBlockWrapper_t)
		{
			.Signature          = CBW_SIGNATURE,
			.DataTransferLength = 0,
			.Flags              = COMMAND_DIRECTION_DATA_IN,
			.LUN                = LUNIndex,
			.SCSICommandLength  = 6,
			.SCSICommandData    =
				{
					SCSI_CMD_TEST_UNIT_READY,
					0x00,                   // Reserved
					0x00,                   // Reserved
					0x00,                   // Reserved
					0x00,                   // Reserved
					0x00                    // Unused (control)
				}
		};
	
	CommandStatusWrapper_t SCSICommandStatus;

	// Send the command and any data to the attached device
	if ((ErrorCode = MassStore_SendCommand(&SCSICommandBlock, NULL)) != 0)
	{
		return ErrorCode;
	}
	
	// Retrieve status information from the attached device
	if ((ErrorCode = MassStore_GetReturnedStatus(&SCSICommandStatus)) != 0)
	{
		return ErrorCode;
	}
	
	// on success it returns 0 
	// on error returns error code
	return ErrorCode;
} /* End MassStore_TestUnitReady() */
/************************************************************************************************************************/

/** Issues a SCSI Device Read Capacity command to the attached device, to determine the capacity of the
 *  given Logical Unit within the device.
 *
 *  \param[in] LUNIndex      Index of the LUN inside the device the command is being addressed to
 *  \param[out] CapacityPtr  Device capacity structure where the capacity data is to be stored
 *
 *  \return A value from the Pipe_Stream_RW_ErrorCodes_t enum, or MASS_STORE_SCSI_COMMAND_FAILED if the SCSI command fails
 */
uint8_t MassStore_ReadCapacity(const uint8_t LUNIndex, SCSI_Capacity_t* const CapacityPtr)
{
	uint8_t ErrorCode;

	// Create a CBW with a SCSI command to issue READ CAPACITY command
	CommandBlockWrapper_t SCSICommandBlock = (CommandBlockWrapper_t)
		{
			.Signature          = CBW_SIGNATURE,
			.DataTransferLength = sizeof(SCSI_Capacity_t),
			.Flags              = COMMAND_DIRECTION_DATA_IN,
			.LUN                = LUNIndex,
			.SCSICommandLength  = 10,
			.SCSICommandData    =
				{
					SCSI_CMD_READ_CAPACITY_10,
					0x00,                   // Reserved
					0x00,                   // MSB of Logical block address
					0x00,
					0x00,
					0x00,                   // LSB of Logical block address
					0x00,                   // Reserved
					0x00,                   // Reserved
					0x00,                   // Partial Medium Indicator
					0x00                    // Unused (control)
				}
		};
	
	CommandStatusWrapper_t SCSICommandStatus;

	// Send the command and any data to the attached device
	if ((ErrorCode = MassStore_SendCommand(&SCSICommandBlock, CapacityPtr)) != 0)
	{
		return ErrorCode;
	}
	  
	// Endian-correct the read data
	CapacityPtr->Blocks    = SwapEndian_32(CapacityPtr->Blocks);
	CapacityPtr->BlockSize = SwapEndian_32(CapacityPtr->BlockSize);
	
	// Retrieve status information from the attached device
	if ((ErrorCode = MassStore_GetReturnedStatus(&SCSICommandStatus)) != 0)
	{
		return ErrorCode;
	}

	return ErrorCode;
} /* End MassStore_ReadCapacity() */
/************************************************************************************************************************/

/** Issues a SCSI Device Prevent/Allow Medium Removal command to the attached device, to lock the physical media from
 *  being removed. This is a legacy command for SCSI disks with removable storage (such as ZIP disks), but should still
 *  be issued before the first read or write command is sent.
 *
 *  \param[in] LUNIndex        Index of the LUN inside the device the command is being addressed to
 *  \param[in] PreventRemoval  Whether or not the LUN media should be locked to prevent removal or not
 *
 *  \return A value from the Pipe_Stream_RW_ErrorCodes_t enum, or MASS_STORE_SCSI_COMMAND_FAILED if the SCSI command fails
 */
uint8_t MassStore_PreventAllowMediumRemoval(const uint8_t LUNIndex, const bool PreventRemoval)
{
	uint8_t ErrorCode;

	// Create a CBW with a SCSI command to issue PREVENT ALLOW MEDIUM REMOVAL command
	CommandBlockWrapper_t SCSICommandBlock = (CommandBlockWrapper_t)
		{
			.Signature          = CBW_SIGNATURE,
			.DataTransferLength = 0,
			.Flags              = COMMAND_DIRECTION_DATA_OUT,
			.LUN                = LUNIndex,
			.SCSICommandLength  = 6,
			.SCSICommandData    =
				{
					SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL,
					0x00,                   // Reserved
					0x00,                   // Reserved
					PreventRemoval,         // Prevent flag
					0x00,                   // Reserved
					0x00                    // Unused (control)
				}
		};
	
	CommandStatusWrapper_t SCSICommandStatus;

	// Send the command and any data to the attached device
	if ((ErrorCode = MassStore_SendCommand(&SCSICommandBlock, NULL)) != 0)
	{
		return ErrorCode;
	}
	
	// Retrieve status information from the attached device
	if ((ErrorCode = MassStore_GetReturnedStatus(&SCSICommandStatus)) != 0)
	{
		return ErrorCode;
	}

	return ErrorCode;
}  /* End MassStore_PreventAllowMediumRemoval() */
/***********************************************************************************************************************/