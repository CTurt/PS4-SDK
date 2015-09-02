/*
 * PlayStation 4 Eye basic sample  
 * If sceCameraIsAttached return 0 and you try to call sceCameraOpen you will received a invalid handle 0xFFFFFFFF... 
 * debug is using udp log: socat udp-recv:18194 stdout on your host
 * change your ip to receive udp logs in sceNetInetPton function
 * It will open camera , it will start  and it will try to get 10 frames, then it will stop and close the camera and return
 * Output:
	debugnet Initialized
	Camera opened with handle: 1
	Camera started led is in on :P
	Frame 0 sceCameraGetFrameData return  0 fine we got a frame
	Frame 1 sceCameraGetFrameData return  0 fine we got a frame
	Frame 2 sceCameraGetFrameData return  0 fine we got a frame
	Frame 3 sceCameraGetFrameData return  0 fine we got a frame
	Frame 4 sceCameraGetFrameData return  0 fine we got a frame
	Frame 5 sceCameraGetFrameData return  0 fine we got a frame
	Frame 6 sceCameraGetFrameData return  0 fine we got a frame
	Frame 7 sceCameraGetFrameData return  0 fine we got a frame
	Frame 8 sceCameraGetFrameData return  0 fine we got a frame
	Frame 9 sceCameraGetFrameData return  0 fine we got a frame
	Camera stop led off
 */
#include "ps4.h"

#define debug(sock, format, ...)\
	do {\
		char buffer[512];\
		int size = sprintf(buffer, format, ##__VA_ARGS__);\
		sceNetSend(sock, buffer, size, 0);\
	} while(0)

int _main(void) {
	// Init and resolve libraries
	initKernel();
	
	initLibc();
	initNetwork();
	initCamera();


	// Connect to server and send message
	char socketName[] = "debug";

	struct sockaddr_in server;

	//udp log to port 18194
	server.sin_len = sizeof(server);
	server.sin_family = AF_INET;
	sceNetInetPton(2, "192.168.1.3", &server.sin_addr);
	server.sin_port = sceNetHtons(18194);
	memset(server.sin_zero, 0, sizeof(server.sin_zero));

	int sock = sceNetSocket(socketName, AF_INET, SOCK_DGRAM, 0);
	sceNetConnect(sock, (struct sockaddr *)&server, sizeof(server));

	debug(sock, "debugnet Initialized\n");
	
	int handle;
	int ret;
	int count;
	int index;
	int modules[256];
	
	//First we check if camera is attached it must return 1 if not we can't follow and return
	ret = sceCameraIsAttached(0);
	if(!ret)
	{
		
		debug(sock, "received error %08x\n", ret);
		return ret;		
		
	}
	//there is a check in libSceCamera.sprx it seems 0x0FF is the userid for service camera
	handle = sceCameraOpen(0x0FF, 0, 0, NULL);
	if(handle < 0)
	{
		debug(sock, "sceCameraOpen received error: %08x\n",handle);
		return handle;
	}
	debug(sock, "Camera opened with handle: %d\n", handle);
	//function check size and some values on unknown1 and unknown2 test and error you will get 802e0000 on bad values
	SceCameraStartParameter param;
	param.size = 0x18;
	param.unknown1 = 0x08;
	param.unknown2 = 0x08;
	
	ret=sceCameraStart(handle, &param);
	if(ret<0)
	{		
		debug(sock, "sceCameraStart received error %08x\n", ret);
		debug(sock, "Stop camera return %08x\n", sceCameraStop(handle));
		debug(sock, "Closing camera return %08x\n", sceCameraClose(handle));
		return ret;	
	}
	debug(sock, "Camera started led is on :P\n");
	//function check size it must be < 0x209 and correct values on unknown1. bad values or incorrect size and you will get get 0x802e0000
	SceCameraFrameData frame;
	frame.size = 0x208;
	frame.unknown1 = 0;
	for(index = 0; index < 10; index++)
	{
		ret=sceCameraGetFrameData(handle, &frame);
		if(ret<0)
		{		
			debug(sock, "Frame %d sceCameraGetFrameData received error %08x\n", index, ret);
		
		}
		else
		{
			debug(sock, "Frame %d sceCameraGetFrameData return  %d fine we got a frame\n", index, ret);
		}
	}
	ret=sceCameraStop(handle);
	if(ret<0)
	{		
		debug(sock, "received error %08x\n", ret);
		
	}
	debug(sock, "Camera stop led off\n");
	
	sceCameraClose(handle);
	

	sceNetSocketClose(sock);


	// Return to browser
	return 0;
}
