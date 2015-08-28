# PS4 SDK
An open source PlayStation 4 SDK.

## Building and installing
Just run `make` on the `PS4-SDK` directory to build the library.

Then create an environment variable called, `PS4SDK` which points to the base directory of the repository, using the Unix path style; for example:

    /C/Users/Chris/Documents/GitHub/PS4-SDK

You must make sure that your compiler supports the System V AMD64 ABI calling convention. PS4-SDK relies on this for compatibility with Sony's functions and system calls.

## Design
At the moment, it is primarily focused on the kernel. For example, [all semaphore system calls have been implemented](https://github.com/CTurt/PS4-SDK/blob/master/source/semaphore.c), from 549 to 556.

However, using function pointers and the `RESOLVE` macro, we can call functions from other modules. For example, [the socket functions](https://github.com/CTurt/PS4-SDK/blob/master/source/network.c) from `libSceNet.sprx`.

## Support
Whilst the SDK isn't capable of most basic functionality yet, such as displaying graphics, or receiving controller input, there are workarounds: rendering to an HTML5 canvas, and using third party USB controllers, or a DS/PSP wirelessly for example.

The SDK is currently in a state where projects like an FTP server, or [a mini Pong game](https://github.com/CTurt/PS4-Pong) could be developed.

## Examples
`hello` - Load `libc` module and perform `getpid` system call

`sockets` - Send a message over a TCP socket

`modules` - Load and dump a module

`threads` - Create and wait for a thread to increment a variable

`canvas` - Render to an HTML5 canvas

`camera` - Initialise the PlayStation Camera and capture 10 frames

`pad` - Read controller input (not finished)

### Filesystem
`directories` - List the contents of a directory

`files` - Read a file's size, and its contents

### USB
`list_devices` - List information about all USB devices connected

`storage` - Read raw image from a USB flash drive