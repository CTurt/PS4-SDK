/*----------------------------------------------------------------------*/
/* Petit FatFs sample project for generic uC  (C)ChaN, 2010             */
/*----------------------------------------------------------------------*/

#include <ps4.h>
#include <usbfatfs.h>

int _netdebug_sock;

#define debug(...)\
	do {\
		char buffer[512];\
		int size = sprintf(buffer, ##__VA_ARGS__);\
		sceNetSend(_netdebug_sock, buffer, size, 0);\
	} while(0)


/*-----------------------------------------------------------------------*/
/* Program Main                                                          */
/*-----------------------------------------------------------------------*/
#define SIZE_OF_BUFFER 512 //64

int _main(void)
{
	// Init and resolve libraries
	initKernel();
	initLibc();
	initNetwork();
	initUsb();

	struct sockaddr_in server;

	server.sin_len = sizeof(server);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = IP(192, 168, 0, 4);
	server.sin_port = sceNetHtons(9023);
	memset(server.sin_zero, 0, sizeof(server.sin_zero));

	_netdebug_sock = sceNetSocket("netdebug", AF_INET, SOCK_STREAM, 0);
	sceNetConnect(_netdebug_sock, (struct sockaddr *)&server, sizeof(server));



	FATFS fatfs;			/* File system object */
	FATFS_DIR dir;				/* Directory object */
	// FILINFO fno;			/* File information object */
	WORD i;
	BYTE buff[SIZE_OF_BUFFER];
	FRESULT rc;

	FATFS FatFs;		/* FatFs work area needed for each volume */
	FATFS_FIL Fil;			/* File object needed for each open file */

	UINT bw, br;

	debug("\nMount a volume.\n");
	rc = f_mount(&fatfs, "", 0);	/* Give a work area to the default drive */
	if (rc) debug("die\n");

	debug("\nOpen a test file (message.txt).\n");

	// open an existing file with read access
	if (f_open(&Fil, "Fuses.txt", FA_READ | FA_OPEN_EXISTING) == FR_OK) 	/* Create a file */
	{
		debug("\nType the file content.\n");
		for (;;)
		{
			rc = f_read(&Fil, &buff, SIZE_OF_BUFFER, &br);
			if (rc || !br) break;			// Error or end of file
			for (i = 0; i < br; i++)		// Type the data
				debug("%c", buff[i]);
		}
		if (rc) debug("die\n");

		f_close(&Fil);								/* Close the file */
	}
	if (rc) debug("die\n");
	debug("File Read Complete.\n");

	debug("\nTest completed.\n");

	disk_deinitialize ();

	sceNetSocketClose(_netdebug_sock);

	return 0;
}



/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/

DWORD get_fattime (void)
{
	return	  ((DWORD)(2010 - 1980) << 25)	/* Fixed to Jan. 1, 2010 */
			| ((DWORD)1 << 21)
			| ((DWORD)1 << 16)
			| ((DWORD)0 << 11)
			| ((DWORD)0 << 5)
			| ((DWORD)0 >> 1);
}
