/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include <ps4.h>
#include "diskio.h"
#include "flash_drive.h"
#include "MassStoreCommands.h"
#include "print_struct.h"
#include "config.h"

/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	uint8_t result;

	if(pdrv == 0)
	{
		result = flash_drive_init();
		if(result == FLASH_DRIVE_INIT_ERR_LIBUSB_OPEN_DEVICE)
		{
			return STA_NODISK;
		}
		else if(result>0)
		{
			return STA_NOINIT;
		}

		return STA_INIT_SUCCESS;
	}
	return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	int result;
	(void)result;

	if(pdrv == 0)
	{
		// get status code (for now we can extend it afterward... need to think about it..)
		// Also need to think about the write protect detection using Request Sense SCSI command.
		if(!MassStore_TestUnitReady(0))
			return STA_NODISK;

		// return success if return value from the MassStore_TestUnitReady() is zero
		return	STA_INIT_SUCCESS;

	}
	//
	return STA_NOINIT;
}

/**
 * [disk_deinitialize  description]
 * @return  [description]
 */
DSTATUS disk_deinitialize (void)
{
	int ret_val;

	// The flash_drive_deinit() releases the interface zero, reattach the kernel driver
	// and closes the libusb library.
	ret_val=  flash_drive_deinit();
	if(ret_val<0)
	{
		return RES_ERROR;
	}
	return RES_OK;

}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	uint8_t result;

	if(pdrv == 0)
	{
		// 512 bytes sector size
		result=MassStore_ReadDeviceBlock(BULK_ONLY_DEFAULT_LUN_NUMBER, sector, count, 512, buff);
		if(result > 0)
		{
			#ifdef DEBUG_DISKIO
			printf("Mass Storage Reading failed.\n");
			#endif
			return RES_ERROR;
		}
		else
		{
			#ifdef DEBUG_DISKIO
			printf("\nMass Storage Read success.\n");
			#endif
			return RES_OK;
		}

	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	int result;

	if(pdrv == 0)
	{
		// write the sector
		result = MassStore_WriteDeviceBlock(BULK_ONLY_DEFAULT_LUN_NUMBER, sector, count, 512, buff);
		if(result > 0)
		{
			#ifdef DEBUG_DISKIO
			printf("MassStore_WriteDeviceBlock Failed.\n");
			#endif
			return RES_ERROR;
		}
		else
		{
			#ifdef DEBUG_DISKIO
			printf("MassStore_WriteDeviceBlock Success.\n");
			#endif
			return RES_OK;
		}
	}
	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
// This function is not needed when _FS_READONLY == 1 and _MAX_SS == _MIN_SS.
#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	int result;

	if(pdrv == 0)
	{
		switch(cmd)
		{
			case CTRL_SYNC :
				//

				break;
			case GET_SECTOR_COUNT :
				//

				break;
			case GET_SECTOR_SIZE :
				//

				break;

			case GET_BLOCK_SIZE :
				//

				break;
			case CTRL_ERASE_SECTOR :

				// erase a part of flash memory
				break;
			default :
				return RES_PARERR;
		}

	}

	return RES_PARERR;
}
#endif
