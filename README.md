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
    556 - int cancelSemaphore(int semaphore, int count, int *threadsReleased);</code></pre>

I've also made use of function pointers and the `kldsym` trick to resolve functions from other modules. For example, we it can use socket related functions from the `libSceNet.sprx` module:

    #define SCENET 0xe

    #define RESOLVE(module, name) getFunctionAddressByName(module, #name, &name)

    int (*socket)(const char *, int, int, int);

    int main(void) {
        RESOLVE(SCENET, socket);

        int sock = socket("test", AF_INET, SOCK_STREAM, 0);

        return 0;
    }

## Support
Whilst the SDK isn't capable of a most basic functionality yet, such as displaying graphics, or receiving controller input, it is a solid foundation which will be improved upon over time, especially once we have code execution to test it.