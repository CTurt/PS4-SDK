# PS4 SDK
An open source PlayStation 4 SDK, developed from running ROP tests.

## Design
At the moment, it is primarily focused on the kernel. For example, I've successfully implemented all semaphore system calls, from 549 to 556:

    549 - int createSemaphore(const char *name, int attributes, int startingCount, int maxCount);
    550 - int removeSemaphore(int semaphore);
    551 - int openSemaphore(const char *name, int oflag, int mode, unsigned int value);
    552 - int closeSemaphore(int semaphore);
    553 - int waitSemaphore(int semaphore, int requiredCount, int *microsecondTimeout);
    554 - int pollSemaphore(int semaphore, int requiredCount);
    555 - int signalSemaphore(int semaphore, int count);
    556 - int cancelSemaphore(int semaphore, int count, int *threadsReleased);

However, using function pointers and `getFunctionAddressByName`, we can call functions from other modules. For example, `strcpy` from `libSceLibcInternal.sprx`, and the socket related functions from `libSceNet.sprx`.

The [hello](https://github.com/CTurt/PS4-SDK/blob/master/examples/hello/source/main.c) example shows how to resolve functions from other modules, and perform system calls.

## Support
Whilst the SDK isn't capable of most basic functionality yet, such as displaying graphics, or receiving controller input, it is a solid foundation which will be improved upon over time.

## Building
Just run `make` on the directory.

You must make sure that your compiler supports the System V AMD64 ABI calling convention. PS4-SDK relies on this for compatibility with Sony's functions and system calls.